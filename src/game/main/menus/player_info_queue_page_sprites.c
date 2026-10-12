#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
extern u8 data_027c0028;

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Queues three sprites in a row at y 32 (x 208, 216, 224) of the player info screen. The
 * first one's tile depends on the page (guess: the page number). */
void PlayerInfo_QueuePageSprites(void) {
  Sprite_QueueAt(1, 208, 32, &data_027c0028, 0x3000,
                 (gMenuState.unk_0206d67c.page + 1) * 2 + 32);
  Sprite_QueueAt(1, 216, 32, &data_027c0028, 0x3000, 52);
  Sprite_QueueAt(1, 224, 32, &data_027c0028, 0x3000, 40);
}
