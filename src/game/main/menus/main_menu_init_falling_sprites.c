#include "types.h"
#include "game/falling_sprites.h"

s32 Rng_Range(s32 min, s32 max);

extern FallingSpriteFrame data_021599e4[];
extern FallingSpriteFrame data_021599f8[];

#define FRAME_FIELD(sprite, field)                            \
  ((sprite)->fast == 0 ? data_021599e4[(sprite)->frame].field \
                       : data_021599f8[(sprite)->frame].field)

/* Gives the 16 falling sprites of the main menu a random table, frame,
 * time into that frame and position (x and y 0-511). */
void MainMenu_InitFallingSprites(FallingSpriteTask* task) {
  FallingSprite* sprite;
  int i;

  for (i = 0; i < FALLING_SPRITE_COUNT; i++) {
    sprite = &task->sprites[i];
    sprite->fast = Rng_Range(0, 1);
    sprite->frame = Rng_Range(0, 7);
    sprite->timer = Rng_Range(0, FRAME_FIELD(sprite, duration) - 1);
    sprite->x = Rng_Range(0, 512);
    sprite->y = Rng_Range(0, 512);
  }
}
