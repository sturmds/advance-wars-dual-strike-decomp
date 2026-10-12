#ifndef GAME_OV005_RIPPLE_H
#define GAME_OV005_RIPPLE_H

#include "types.h"

/* Overlay 5 effect that shifts each screen line sideways by its own amount.
 * The offsets are fx32 values in tables of one entry per line. */
#define OV005_RIPPLE_LINES 192

void Ov005Ripple_FillOffsets(int* offsets, int value);
void Ov005Ripple_AddWave(int* offsets, int amplitude, int step, float phase);
void Ov005Ripple_PackOffsets(int* offsets, s16* packed);

#endif
