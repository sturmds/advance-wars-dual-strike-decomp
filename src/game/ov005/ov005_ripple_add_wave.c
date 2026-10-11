#include "types.h"
#include "game/ov005_ripple.h"

extern s16 gSineTable[];            /* looks like a 256-entry sine table */

/* Adds a sine wave to the line offsets: line i reads the table at
 * (phase + i * step) >> 12, scaled by amplitude. `phase` is a float because
 * the original computes the angle with the compiler's software float. */
void Ov005Ripple_AddWave(int *offsets, int amplitude, int step, float phase)
{
    int i = 0;
    int angle = i;

    for (; i < OV005_RIPPLE_LINES; i++) {
        offsets[i] += (amplitude * gSineTable[((int)(phase + angle) >> 12) & 0xff]) >> 12;
        angle += step;
    }
}
