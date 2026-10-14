#include "types.h"
#include "game/task.h"
#include "game/link_marker_task.h"
#include "game/screen_other_map.h"
#include "game/oam_cells.h"

s32 HwDiv32(s32 numer, s32 denom);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(Task* task);

/* Needed to match: const lets mwcc load the sine and cosine once. */
extern const s16 gSineTable[];

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Task step of a link menu marker, 5 frames, on the screen of data_027c0288.unk_00:
 * draws the marker through affine set affineIndex at scale / 256 of its height and
 * (512 - scale) / 256 of its width, scale easing from 256 down to 2, so the marker
 * squashes into a flat line twice as wide. Then resumes the script. */
void LinkMarkerTask_SquashOut(LinkMarkerTask* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int scale;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (task->frame > 4) {
    Task_ResumeScript((Task*)task);
  } else {
    scale = Math_Ease(1, 0x100, 2, task->frame, 4);
    {
      int heightDiv2 = DIVISOR(scale);
      int widthDiv2 = DIVISOR(0x200 - scale);
      int heightDiv = DIVISOR(scale);
      int widthDiv = DIVISOR(0x200 - scale);

      Oam_SetAffineParams(task->affineIndex,
                          HwDiv32(gSineTable[64] * 16, widthDiv),
                          HwDiv32(-gSineTable[0] * 16, heightDiv),
                          HwDiv32(gSineTable[0] * 16, widthDiv2),
                          HwDiv32(gSineTable[64] * 16, heightDiv2));
    }
    Sprite_Queue(0, ((task->x - 16) & 0x1ff) | (task->affineIndex << 9),
                 ((task->y - 8) & 0xff) | 0x300, &data_027c0060, 0, 52);
    task->frame++;
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
