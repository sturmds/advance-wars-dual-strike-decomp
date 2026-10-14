#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, const void* data, int unk08,
                    int unk0a);
int VsSetup_GetSlotTileX(int player);

extern MenuState gMenuState;

#define MENU ((MenuState*)&gMenuState)

/* With more than two player slots, queues a team marker per slot into sprite
 * list 1 of screen 1 at y 136: tile 52 + team * 32 (teams[] is 0-3), x from
 * the slot position. */
void VsSetup_DrawSettingsTeamSprites(void) {
  int player;
  Unk_027c0280* saved;

  if (MENU->setup.unk_2e3 <= 2) {
    return;
  }
  saved = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[1]);
  for (player = 0; player < MENU->setup.unk_2e3; player++) {
    Sprite_QueueAt(1, (VsSetup_GetSlotTileX(player) * 8) & 0x1ff, 136,
                   &data_027c0008, 0x1000, MENU->setup.teams[player] * 32 + 52);
  }
  Screen_SetCurrentGfx(saved);
}
