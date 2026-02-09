#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "conv.h"
#include "wav.h"
#include "iir.h"
#include "adaptive_filter.h"
// #include "low_pass_8th_order.h"
#include "low_pass_32th_order.h"
#include "IIR_low_pass_filters.h"
#include "fir.h"
#include "stdint.h"

#define N 48000
#define AUDIO_IO_SIZE 256
#define INPUT_FILE "../../streams/WhiteNoise.wav"

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
int16_t bufferL[AUDIO_IO_SIZE];
int16_t bufferR[AUDIO_IO_SIZE];
int16_t adaptiveCoeffs[FILTER_ORDER];
int16_t noiseFilter[AUDIO_IO_SIZE];
int16_t history[FILTER_ORDER];
int16_t historyFIR[LP_ORDER];
int16_t history_x[2][3];
int16_t history_y[2][3];

int main(void)
{
    const char *input_filename  = INPUT_FILE;
    const char *output_filename = "output/output.wav";

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
        // No changes
        for (size_t i = 0; i < frames_read; i++) {
            bufferL[i] = inputL[i];
            bufferR[i] = inputR[i];
        }

        // Interleave and write
        for (size_t i = 0; i < frames_read; i++) {
            interleavedOut[2*i]     = bufferL[i];
            interleavedOut[2*i + 1] = bufferR[i];
        }
        fwrite(interleavedOut, input_hdr.blockAlign, frames_read, fout);
        total_frames_written += frames_read;
    }

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
