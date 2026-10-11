#include "types.h"
#include "game/Unk_02080440.h"
#include "game/vision.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020c57bc(s16 x, s16 y, s16 range, s8 *grid, s8 delta, u8 player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Adds delta to every tile within range of (x, y) in the grids gridMask picks
 * (see game/vision.h); player is passed on for its skill checks. */
void Vision_AddArea(s16 x, s16 y, s16 range, int gridMask, s8 delta, int player)
{
    switch (gridMask) {
    case 0:
        break;
    case 1:
        func_020c57bc(x, y, range, VISION_GRID(STATE, STATE->unk_20), delta, player);
        break;
    case 3:
        func_020c57bc(x, y, range, VISION_GRID(STATE, STATE->unk_20), delta, player);
    case 2:
        func_020c57bc(x, y, range, VISION_GRID(STATE, STATE->unk_22), delta, player);
        break;
    }
}
