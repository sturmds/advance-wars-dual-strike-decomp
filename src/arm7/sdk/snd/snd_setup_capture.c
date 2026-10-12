#include "sdk/snd_hw.h"

/*
 * Programs sound capture 0 or 1. Control bits: 0 output (replace or add to
 * the channel), 1 input source, 2 one-shot, 3 format (PCM8 or PCM16).
 */
void SND_SetupCapture(int capture, int format, void *buffer, int length, BOOL loop, int in, int out)
{
    int blockOffset = capture << 3;

    /* needed to match: an int 0/1 ternary folds into a compare with the moves swapped; enum constants do not */
    (&AUDIO_RECORD0_CTRL)[capture] =
        (format << 3) | ((loop ? AUDIO_RECORD_WRAPAROUND : AUDIO_RECORD_SINGLE_PASS) << 2) | (in << 1) | out;
    AUDIO_RECORD_TARGET_ADDR(blockOffset) = (u32)buffer;
    AUDIO_RECORD_WORD_COUNT(blockOffset) = length;
}
