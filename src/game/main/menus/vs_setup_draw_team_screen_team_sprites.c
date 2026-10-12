#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

void Sprite_QueueAt(int list, int x, int y, const void* data, int unk08,
                    int unk0a);
int VsSetup_GetSlotTileX(int player);

extern MenuState gMenuState;
extern u8 data_027c0008;

#define MENU ((MenuState*)&gMenuState)

/* Queues a team marker per player slot into sprite list 1 of screen 0 at
 * y 64: tile 52 + team * 32, x from the slot position. Like
 * VsSetup_DrawSettingsTeamSprites, for the other screen. */
void VsSetup_DrawTeamScreenTeamSprites(void) {
  int player;
  Unk_027c0280* saved = data_027c0280;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  for (player = 0; player < MENU->setup.unk_2e3; player++) {
    Sprite_QueueAt(1, (VsSetup_GetSlotTileX(player) * 8) & 0x1ff, 64,
                   &data_027c0008, 0x1000, MENU->setup.teams[player] * 32 + 52);
  }
  Screen_SetCurrentGfx(saved);
}
