#include "types.h"
#include "game/row_lock_mark_task.h"
#include "game/screen_other_map.h"

s32 HwDiv32(s32 numerator, s32 denominator);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Task_ResumeScript(RowLockMarkTask* task);

extern const s16 gSineTable[];
extern u8 data_027c0060;

#define DIVISOR(scale) ((scale) ? (scale) : 2)

#define SQUASH_FRAMES 4
#define MARK_TILE 0x124

/* Task script step that removes a row's lock mark after the row is
 * unlocked: over SQUASH_FRAMES frames the mark stretches to twice its width
 * while its height shrinks to almost nothing, drawn as a double-size affine
 * sprite. On the frame after that the script resumes. */
void RowLockMark_SquashOut(RowLockMarkTask* task) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedState;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  if (task->frame > SQUASH_FRAMES) {
    Task_ResumeScript(task);
  } else {
    int scaleY = Math_Ease(1, 256, 2, task->frame, SQUASH_FRAMES);
    int d3 = DIVISOR(scaleY);
    int d2 = DIVISOR(512 - scaleY);
    int d1 = DIVISOR(scaleY);
    int d0 = DIVISOR(512 - scaleY);

    Oam_SetAffineParams(task->affineIndex, HwDiv32(gSineTable[64] * 16, d0),
                        HwDiv32(-gSineTable[0] * 16, d1),
                        HwDiv32(gSineTable[0] * 16, d2),
                        HwDiv32(gSineTable[64] * 16, d3));
    /* Affine double-size sprite (0x300): the 32x16 box grows to 64x32, so x and y move back by 16 and 8. */
    Sprite_Queue(1, ((task->x - 16) & 0x1ff) | (task->affineIndex << 9),
                 ((task->y - 8) & 0xff) | 0x300, &data_027c0060, 0, MARK_TILE);
    task->frame++;
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
