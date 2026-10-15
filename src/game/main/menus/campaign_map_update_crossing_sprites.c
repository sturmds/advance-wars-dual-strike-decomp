#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_state.h"
#include "game/Unk_02088d04.h"

void SpriteAnim_Draw(struct ShowSprite* sprite, int x, int y);
s32 Rng_Range(s32 min, s32 max);

#define SCROLL_X (data_027c0280->bgScrollX[data_027c0280->layer3Bg])
#define SCROLL_Y (data_027c0280->bgScrollY[data_027c0280->layer3Bg])

/* States of each sprite (unk_64 for sprite 0, unk_66 for sprite 1). */
#define CROSS_ENTER 0 /* place it at its edge of the screen */
#define CROSS_MOVING 1
#define CROSS_WAIT 2 /* off screen; counts down sprite0Wait or sprite1Wait */

/* Update function of the two CampaignMapWork.sprites that cross the
 * world map. This task keeps their map positions in fromX/fromY (sprite 0)
 * and toX/toY (sprite 1). Sprite 0 enters at the left edge at screen y
 * 112 to 207 and moves 2 pixels right per pixel up; sprite 1 enters below
 * the bottom edge at screen x 0 to 127 and moves 2 pixels up per pixel
 * right. Once a sprite leaves the screen at the right or top, it waits 30
 * to 127 frames and enters again. */
void CampaignMap_UpdateCrossingSprites(CampaignMapTask* task) {
  if (task->unk_64 == CROSS_MOVING) {
    SpriteAnim_Draw(gMenuState.campaignMap.sprites[0],
                    (task->fromX - SCROLL_X) & 0x1ff,
                    (task->fromY - SCROLL_Y) & 0xff);
  }
  if (task->unk_66 == CROSS_MOVING) {
    SpriteAnim_Draw(gMenuState.campaignMap.sprites[1],
                    (task->toX - SCROLL_X) & 0x1ff,
                    (task->toY - SCROLL_Y) & 0xff);
  }
  if (task->step & 2) {
    if (task->step & 1) {
      task->fromX++;
      task->toX++;
    } else {
      task->fromY--;
      task->toY--;
    }
  } else if (task->step & 1) {
    task->fromX++;
  } else {
    task->toY--;
  }
  switch (task->unk_64) {
    case CROSS_ENTER: {
      /* needed to match: block locals keep the state pointer and BG
         * index in the original registers across the call */
      Unk_027c0280* gfx;
      int bg;
      int offset;

      task->fromX = SCROLL_X - 16;
      gfx = data_027c0280;
      bg = gfx->layer3Bg;
      offset = Rng_Range(-96, 0);
      task->fromY = offset + gfx->bgScrollY[bg] + 208;
      task->unk_64 = CROSS_MOVING;
      break;
    }
    case CROSS_MOVING:
      if (task->fromX - SCROLL_X > 272 || task->fromY - SCROLL_Y < -16) {
        task->unk_64 = CROSS_WAIT;
        task->sprite0Wait = Rng_Range(30, 128);
      }
      break;
    case CROSS_WAIT:
      if (task->sprite0Wait == 0) {
        task->unk_64 = CROSS_ENTER;
      }
      task->sprite0Wait--;
      break;
  }
  switch (task->unk_66) {
    case CROSS_ENTER: {
      Unk_027c0280* gfx;
      int bg;
      int offset;

      gfx = data_027c0280;
      bg = gfx->layer3Bg;
      offset = Rng_Range(0, 128);
      task->toX = offset + gfx->bgScrollX[bg];
      task->toY = SCROLL_Y + 192;
      task->unk_66 = CROSS_MOVING;
      break;
    }
    case CROSS_MOVING:
      if (task->toX - SCROLL_X > 272 || task->toY - SCROLL_Y < -16) {
        task->unk_66 = CROSS_WAIT;
        task->sprite1Wait = Rng_Range(30, 128);
      }
      break;
    case CROSS_WAIT:
      if (task->sprite1Wait == 0) {
        task->unk_66 = CROSS_ENTER;
      }
      task->sprite1Wait--;
      break;
  }
  task->step++;
}
