#include "types.h"
#include "game/Unk_02080440.h"
#include "game/BattleCellTask.h"
#include "game/build_menu.h"
#include "game/map_data.h"
#include "game/acting_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
u32 gLinkPlay;
s16 data_0229eae0[2];
u16 data_0229ead8[2];
s16 data_0229ead4[2];
MapData* data_0229eadc;
int data_0229eae4;
int gBuildMenuCount;                    /* entries in gBuildMenuEntries */
u8 gSlotControllers[0x8];               /* size: up to the next symbol */
BuildMenuEntry gBuildMenuEntries[0x1c]; /* size: up to the next symbol */
MenuState gMenuState;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Puts back the acting unit start tile that ActingUnit_SaveStartPos saved in data_0229ead4. */
void ActingUnitStart_Restore(void) {
  STATE->actingUnitStartX = data_0229ead4[0];
  STATE->actingUnitStartY = data_0229ead4[1];
}
