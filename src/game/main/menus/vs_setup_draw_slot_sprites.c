#include "types.h"
#include "game/menu_state.h"
#include "game/player.h"
#include "game/screen_other_map.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, const void* data, int unk08,
                    int unk0a);
void CoPicture_QueueSprite(int x, int y, int unk08, int unk0a, int list);
int VsSetup_GetSlotTileX(int player);
int VsSetup_GetPlayerOfSlot(int slot);

extern MenuState gMenuState;

#define STATE (data_027c027c->unk_48[data_027c0284])
/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Queues the sprites of each player slot of the Vs setup screen into the
 * sprite lists of screen 1. At y 56, one controller marker per map: tile
 * VsSetup_GetPlayerOfSlot(slot) * 4 for a human, tile 16 for the AI. At y 80, the CO
 * frame, then the front and back CO pictures that VsSetup_SetCo loaded
 * (index = player * 2 + 0 or 1: tile 552 + index * 18, palette index + 2). */
void VsSetup_DrawSlotSprites(void) {
  int player;
  Unk_027c0280* saved = data_027c0280;

  Screen_SetCurrentGfx((&data_027c0288.unk_00)[1]);
  if (STATE->twoMaps == 0) {
    for (player = 0; player < MENU->setup.unk_2e3; player++) {
      if (MENU->setup.controllers[player][0] != PLAYER_CONTROLLER_AI) {
        Sprite_QueueAt(1, (VsSetup_GetSlotTileX(player) * 8 + 24) & 0x1ff, 56,
                       &data_027c0040, 0,
                       VsSetup_GetPlayerOfSlot(player + 1) * 4);
      } else {
        Sprite_QueueAt(1, (VsSetup_GetSlotTileX(player) * 8 + 24) & 0x1ff, 56,
                       &data_027c0040, 0, 16);
      }
    }
  } else {
    for (player = 0; player < MENU->setup.unk_2e3; player++) {
      if (MENU->setup.controllers[player][0] != PLAYER_CONTROLLER_AI) {
        Sprite_QueueAt(0, (VsSetup_GetSlotTileX(player) * 8 + 12) & 0x1ff, 56,
                       &data_027c0040, 0,
                       VsSetup_GetPlayerOfSlot(player + 1) * 4);
      } else {
        Sprite_QueueAt(0, (VsSetup_GetSlotTileX(player) * 8 + 12) & 0x1ff, 56,
                       &data_027c0040, 0, 16);
      }
      if (MENU->setup.controllers[player][1] != PLAYER_CONTROLLER_AI) {
        Sprite_QueueAt(1, (VsSetup_GetSlotTileX(player) * 8 + 36) & 0x1ff, 56,
                       &data_027c0040, 0,
                       VsSetup_GetPlayerOfSlot(player + 1) * 4);
      } else {
        Sprite_QueueAt(1, (VsSetup_GetSlotTileX(player) * 8 + 36) & 0x1ff, 56,
                       &data_027c0040, 0, 16);
      }
    }
  }
  for (player = 0; player < MENU->setup.unk_2e3; player++) {
    Sprite_QueueAt(1, (VsSetup_GetSlotTileX(player) * 8 + 8) & 0x1ff, 80,
                   &data_027c01dc, 0x1000, 0x204);
    if (MENU->setup.cos[player][0] != 0) {
      CoPicture_QueueSprite((VsSetup_GetSlotTileX(player) * 8 + 8) & 0x1ff, 80,
                            (player * 2 + 2) << 12, 552 + player * 2 * 18, 2);
    }
    if (MENU->setup.cos[player][1] != 0) {
      CoPicture_QueueSprite((VsSetup_GetSlotTileX(player) * 8 + 32) & 0x1ff, 80,
                            (player * 2 + 3) << 12, 552 + (player * 2 + 1) * 18,
                            2);
    }
  }
  Screen_SetCurrentGfx(saved);
}
