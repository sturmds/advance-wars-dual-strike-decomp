#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s16
    data_0229ead4[2]; /* x, y; ActingUnitStart_Restore copies them back */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Saves the tile the acting unit started its move on. */
void ActingUnit_SaveStartPos(void) {
  data_0229ead4[0] = STATE->actingUnitStartX;
  data_0229ead4[1] = STATE->actingUnitStartY;
}
