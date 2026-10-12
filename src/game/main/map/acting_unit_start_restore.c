#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s16 data_0229ead4[2];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Puts back the acting unit start tile that ActingUnit_SaveStartPos saved in data_0229ead4. */
void ActingUnitStart_Restore(void) {
  STATE->actingUnitStartX = data_0229ead4[0];
  STATE->actingUnitStartY = data_0229ead4[1];
}
