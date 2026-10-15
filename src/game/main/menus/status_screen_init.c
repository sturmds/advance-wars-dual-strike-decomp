#include "types.h"
#include "game/icon.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Gfx_ClearLayerMap(int layer);
u8 Map_GetLoadedPlayerCount(void);
void File_UnpackTo(void* dest, const char* fileName);
void Font_LoadDigits(int baseTile);
void StatusScreen_Draw(void);
void Gfx_RequestLayerCopy(int layer);
void StatusScreen_AddButton(void);

extern char data_0215fba0[]; /* status screen background file */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* Status screen setup: an army icon for each undefeated player (OBJ tiles
 * 404 on, 8 per row, where StatusScreen_QueueSprites shows them), the
 * background on layer 1, the table, then the back button. */
void StatusScreen_Init(void) {
  int i;

  Gfx_ClearLayerMap(0);
  Gfx_ClearLayerMap(1);
  for (i = 0; i < Map_GetLoadedPlayerCount(); i++) {
    if (MAP_STATE->players[i + 1].defeated == 0) {
      Icon_LoadTo(ICON_ARMY_BANNERS - 1 + MAP_STATE->players[i + 1].armyColour,
                  data_027c0280->objVram, 404 + i * 8);
    }
  }
  IconBank_LoadPalette(1, 22);
  Icon_LoadTo(2, data_027c0280->objVram, 436);
  IconBank_LoadPalette(0, 21);
  File_UnpackTo(data_027c0280->layer1Map, data_0215fba0);
  Font_LoadDigits(0);
  StatusScreen_Draw();
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  StatusScreen_AddButton();
}
