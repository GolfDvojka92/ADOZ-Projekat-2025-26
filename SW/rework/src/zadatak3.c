#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include "../inc/direct_form_iir.h"
#include "../inc/wav.h"
#include "../inc/high_pass_4350Hz.h"
#include "../inc/fir.h"
#include "../inc/notch.h"

#define N 48000
#define AUDIO_IO_SIZE 256
#define INPUT_FILE "../../streams/17.wav"

static void ensure_output_dir(void) {
    struct stat st = {0};
    if (stat("output", &st) == -1) {
        if (mkdir("output", 0755) != 0) { perror("mkdir"); exit(EXIT_FAILURE); }
    }
}

// Per-channel FIR state
static uint16_t stateL = 0;
static uint16_t stateR = 0;

int16_t interleavedIn[AUDIO_IO_SIZE * 2];
int16_t interleavedOut[AUDIO_IO_SIZE * 2];
int16_t inputL[AUDIO_IO_SIZE];
int16_t inputR[AUDIO_IO_SIZE];
int16_t tempL[AUDIO_IO_SIZE];
int16_t tempR[AUDIO_IO_SIZE];
int16_t bufferL[AUDIO_IO_SIZE];
int16_t bufferR[AUDIO_IO_SIZE];
int16_t historyFIR_L[H_PASS_ORDER];
int16_t historyFIR_R[H_PASS_ORDER];
int16_t history_X_L[3];
int16_t history_X_R[3];
int16_t history_Y_L[3];
int16_t history_Y_R[3];
int16_t b_coeffs[3];
int16_t a_coeffs[3];

int main(void)
{
    const char *input_filename  = INPUT_FILE;
    const char *output_filename = "output/output2.wav";

    ensure_output_dir();

    FILE *fin = fopen(input_filename, "rb");
    if (!fin) { perror("fopen input"); return EXIT_FAILURE; }

    WAVHeader input_hdr;
    if (read_wav_header(fin, &input_hdr) != 0) {
        fprintf(stderr, "Failed to read WAV header from %s\n", input_filename);
        fclose(fin);
        return EXIT_FAILURE;
    }

    if (input_hdr.audioFormat != 1 || input_hdr.bitsPerSample != 16 || input_hdr.numChannels != 2) {
        fprintf(stderr, "Unsupported WAV format: need 16-bit stereo PCM\n");
        fclose(fin);
        return EXIT_FAILURE;
    }

    WAVHeader output_hdr = input_hdr;
    FILE *fout = fopen(output_filename, "wb+");
    if (!fout) { perror("fopen output"); fclose(fin); return EXIT_FAILURE; }

    // Placeholder header; we’ll fix sizes after writing all samples
    fwrite(&output_hdr, sizeof(WAVHeader), 1, fout);

    size_t total_frames_written = 0;

    size_t frames_read;

    while ((frames_read = fread(interleavedIn, input_hdr.blockAlign, AUDIO_IO_SIZE, fin)) > 0)
    {
        // Deinterleave
        for (size_t i = 0; i < frames_read; i++) {
            inputL[i] = interleavedIn[2*i];
            inputR[i] = interleavedIn[2*i + 1];
        }

        // Filter ALL frames_read, not half
        for (size_t i = 0; i < frames_read; i++) {
            bufferL[i] = fir_circular(inputL[i], high_pass_35th_order, historyFIR_L, H_PASS_ORDER, &stateL);
            bufferR[i] = fir_circular(inputR[i], high_pass_35th_order, historyFIR_R, H_PASS_ORDER, &stateR);
        }
        
        // Generate notch filter coeffs
        generate_notch_coeffs(N, 2000, 0.95, b_coeffs, a_coeffs);

        // Filter the frames received from the FIR filter
        for (size_t i = 0; i < frames_read; i++) {
            bufferL[i] = iir_basic(bufferL[i], b_coeffs, history_X_L, 3, a_coeffs, history_Y_L, 3);
            bufferR[i] = iir_basic(bufferR[i], b_coeffs, history_X_R, 3, a_coeffs, history_Y_R, 3);
        }

        // Interleave and write
        for (size_t i = 0; i < frames_read; i++) {
            interleavedOut[2*i]     = bufferL[i];
            interleavedOut[2*i + 1] = bufferR[i];
        }
        fwrite(interleavedOut, input_hdr.blockAlign, frames_read, fout);
        total_frames_written += frames_read;
    }
    printf("Notch filter coeffitients:\n");
    for (int i = 0; i < 3; i++) {
        printf("\tb%d: %5d\ta%d: %5d\n", i, b_coeffs[i], i, a_coeffs[i]);
    }
    printf("Generated output2.wav successfully\n");

    // ---- Fix header sizes now that we know total_frames_written ----
    // Size in bytes of the data chunk:
    output_hdr.subchunk2Size = (uint32_t)(total_frames_written * input_hdr.blockAlign);
    output_hdr.chunkSize     = 36 + output_hdr.subchunk2Size;

    // Seek to file start and rewrite header
    fseek(fout, 0, SEEK_SET);
    fwrite(&output_hdr, sizeof(WAVHeader), 1, fout);

    fclose(fout);
    fclose(fin);

    return 0;
}
