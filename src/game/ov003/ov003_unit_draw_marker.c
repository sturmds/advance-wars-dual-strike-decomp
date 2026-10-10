#include "types.h"
#include "game/ov003_unit.h"

s32 HwMod32(s32 numer, s32 denom);
int func_ov003_0235da18(int row);
void Ov003_DrawTexturedQuad(int x, int y, int a, int b, int c, int d, int e, int f, int g);

/* Draws a four-frame animation above a unit whose row passes
 * func_ov003_0235da18. It shows in the first 8 frames of every 20 (counter
 * Ov003State.frameCount); frames last 1, 2, 3 and 2 ticks. */
void Ov003Unit_DrawMarker(Ov003Unit *unit)
{
    int phase;

    if (func_ov003_0235da18(unit->unk_12_3) == 0) {
        return;
    }
    phase = HwMod32(data_ov003_02364928->frameCount, 20);
    if (phase < 1) {
        phase = 0;
    } else if (phase < 3) {
        phase = 1;
    } else if (phase < 6) {
        phase = 2;
    } else if (phase < 8) {
        phase = 3;
    } else {
        return;
    }
    Ov003_DrawTexturedQuad((unit->x >> 3) - OV003_SCROLL_X - 16, (unit->y >> 3) - OV003_SCROLL_Y - 22, 24, 2, 2, 0x4e5 + phase * 16, 20, 0, 0);
}
