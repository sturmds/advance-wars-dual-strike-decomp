#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_02080440_48 data_02183b08;
extern u16 gPowerPanelPlayer;
extern u16 data_0213fdcc[16];
extern u16 data_0213fdec[16];

BOOL PowerMeter_IsShown(int screen);
BOOL CanUseCoPower(int player, int slot);
void Palette_LoadWithCopy(const void* src, u16 offset, u32 size);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Cycles colour 15 of palette 23 (offset 0x2fe) through 16 colours, one step
 * every 4 frames, for the player the power panel drew last
 * (gPowerPanelPlayer). The table depends on whether that player can use a CO
 * power. Does nothing outside players 1 to 4. */
void PowerPanel_AnimatePaletteColour(int frame) {
  if (gPowerPanelPlayer < 1 || gPowerPanelPlayer > 4) {
    return;
  }
  if (!PowerMeter_IsShown(STATE == &data_02183b08 ? MAP_STATE_FIRST
                                                  : MAP_STATE_SECOND)) {
    return;
  }
  if (CanUseCoPower(gPowerPanelPlayer, 0)) {
    Palette_LoadWithCopy(&data_0213fdcc[(frame >> 2) & 15], 0x2fe, 2);
  } else {
    Palette_LoadWithCopy(&data_0213fdec[(frame >> 2) & 15], 0x2fe, 2);
  }
}
