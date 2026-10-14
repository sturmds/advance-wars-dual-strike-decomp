#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
extern u8 data_027c0040;

void Sprite_QueueDefaultTile(int x, int y, int attr, int tile, int list);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void PlayerInfo_DrawRankAndExperience(void);

#define MENU (gMenuState.unk_0206d67c)

/* Sprites of the player info screen on the CO portrait's screen: one
 * sprite at the top left, then up to three rows, each a sprite at x 8 and
 * MENU.unk_09d[row] icons 16 pixels apart; then the rank and experience
 * (PlayerInfo_DrawRankAndExperience). */
void PlayerInfo_DrawPortraitScreenSprites(void) {
  int row;
  int icon;

  Sprite_QueueDefaultTile(8, 17, 0x1000, 0, 0);
  for (row = 0; row < (int)ARRAY_COUNT(MENU.unk_09d); row++) {
    if (MENU.unk_09d[row] == 0) {
      break;
    }
    Sprite_QueueDefaultTile(8, (65 + row * 16) & 0xff, 0x1000, 12 + row * 12,
                            0);
    for (icon = 0; icon < MENU.unk_09d[row]; icon++) {
      Sprite_QueueAt(0, (56 + icon * 16) & 0x1ff, (64 + row * 16) & 0xff,
                     &data_027c0040, 0x2000, 0x68);
    }
  }
  PlayerInfo_DrawRankAndExperience();
}
