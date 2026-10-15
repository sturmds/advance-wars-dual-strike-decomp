#include "types.h"
#include "game/task.h"
#include "game/oam_cells.h"
#include "game/oam_buffer.h"
#include "game/math.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(Task* task);

#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) != 0 ? (scale) : 2)

/* OBJ affine matrix of the sprite; OAM attribute 1 bits 9-13. */
#define ZOOM_AFFINE 1

/* OAM attribute 0 bits 8-9: affine sprite drawn in a double-size area. */
#define OAM_AFFINE_DOUBLE 0x300

/* Task script step (data_0215bb88, started by TitleCall_NumberSlideIn):
 * over 32 frames, zooms one sprite at (64, 32) from almost nothing to twice
 * its size through affine matrix 1, then resumes the script. gSineTable
 * has 256 steps per turn (index 64 = cosine of 0). */
void TitleCall_SpriteZoomIn(Task* task) {
  int scale;

  scale = Math_Ease(4, 2, 512, FRAME(task), 32);
  /* Needed to match: the divisor is evaluated once per entry, before the
     * table reads. */
  {
    int d3 = DIVISOR(scale);
    int d2 = DIVISOR(scale);
    int d1 = DIVISOR(scale);
    int d0 = DIVISOR(scale);
    int sinValue = gSineTable[0];
    int cosValue = gSineTable[64] << 4;

    Oam_SetAffineParams(ZOOM_AFFINE, HwDiv32(cosValue, d0),
                        HwDiv32(-sinValue << 4, d1), HwDiv32(sinValue << 4, d2),
                        HwDiv32(cosValue, d3));
  }
  Sprite_Queue(1, 64 | (ZOOM_AFFINE << 9), 32 | OAM_AFFINE_DOUBLE,
               &data_027c0030, 0, 80);
  if (FRAME(task) != 32) {
    FRAME(task)++;
    return;
  }
  Task_ResumeScript(task);
}
