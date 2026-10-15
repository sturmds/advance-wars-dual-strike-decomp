#include "types.h"
#include "game/sprite_anim.h"
#include "game/capture_picture_task.h"
#include "game/oam_buffer.h"
#include "game/math.h"

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Frame step of the task shown when a capture completes: places its sprite
 * above (x, y) through affine matrix 0, which grows it from nothing to full
 * size over 10 frames (an ease from 0 to 0x100). Same matrix shape as
 * DigitSprite_QueuePopIn; gSineTable has 256 steps per turn. */
void CaptureDone_Draw(CaptureDoneTask* task) {
  int scale = Math_Ease(1, 0, 0x100, task->frame, 10);

  SpriteAnim_Draw(task->sprite, task->x & 0x1ff,
                  ((task->y - 16) & 0xff) | 0x100);
  /* Needed to match: the divisor is evaluated once per entry, before the
     * table reads. */
  {
    int d3 = DIVISOR(scale);
    int d2 = DIVISOR(scale);
    int d1 = DIVISOR(scale);
    int d0 = DIVISOR(scale);
    int sinValue = gSineTable[0];
    int cosValue = gSineTable[64] * 16;

    Oam_SetAffineParams(0, HwDiv32(cosValue, d0), HwDiv32(-sinValue * 16, d1),
                        HwDiv32(sinValue * 16, d2), HwDiv32(cosValue, d3));
  }
  if (task->frame < 10) {
    task->frame++;
  }
}
