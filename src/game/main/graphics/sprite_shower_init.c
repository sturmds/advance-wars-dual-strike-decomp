#include "types.h"
#include "game/falling_sprites.h"

s32 Rng_Range(s32 min, s32 max);

extern FallingSpriteFrame data_02159c50[];
extern FallingSpriteFrame data_02159c64[];

#define FRAME_FIELD(sprite, field)                               \
  ((sprite)->variant == 0 ? data_02159c64[(sprite)->frame].field \
                          : data_02159c50[(sprite)->frame].field)

/* Starts the 16 shower sprites: each gets a random table, frame, time into
 * that frame and position (x 528-864, y 128-256: above both screens, which
 * start at y 313). */
void SpriteShower_Init(ShowerSpriteTask* task) {
  ShowerSprite* sprite;
  int i;

  for (i = 0; i < FALLING_SPRITE_COUNT; i++) {
    sprite = &task->sprites[i];
    sprite->active = 1;
    sprite->variant = Rng_Range(0, 1);
    sprite->frame = Rng_Range(0, 7);
    sprite->timer = Rng_Range(0, FRAME_FIELD(sprite, duration) - 1);
    sprite->x = Rng_Range(528, 864);
    sprite->y = Rng_Range(128, 256);
  }
}
