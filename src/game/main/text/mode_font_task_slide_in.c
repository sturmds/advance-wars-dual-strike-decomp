#include "types.h"
#include "game/task.h"
#include "game/mode_font_task.h"
#include "game/oam_cells.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
int HwDiv32(int num, int den);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(Task* task);

/* Sine table, index 64 = cosine of 0. Needed to match: const lets mwcc
 * load the sine and cosine once. */
extern const s16 gSineTable[];

#define SIN0 (gSineTable[0])
#define COS0 (gSineTable[64])

/* Affine parameter set the letters use, and its OAM attribute 1 bits. */
#define LETTER_AFFINE 31
#define LETTER_AFFINE_INDEX (LETTER_AFFINE << 9)

/* OAM attribute 0 bit: affine sprite. */
#define OAM_AFFINE 0x100

#define SLIDE_FRAMES 8

/* Task step, 56 frames: draws the letters at a quarter height. Each letter
 * appears at its letterStart frame 256 pixels to the right of its place and
 * slides there in 8 frames. Drawing stops at the first letter not started
 * yet or still off screen. Then resumes the script. */
void ModeFontTask_SlideIn(ModeFontTask* task) {
  int i;
  u16 x = task->x;
  u16 y = task->y;
  int elapsed;
  int letterX;

  task->frame++;
  Oam_SetAffineParams(LETTER_AFFINE, HwDiv32(COS0 << 4, 256),
                      HwDiv32(-SIN0 << 4, 64), HwDiv32(SIN0 << 4, 256),
                      HwDiv32(COS0 << 4, 64));
  for (i = 0; i < task->length; i++) {
    if (task->frame < task->letterStart[i]) {
      break;
    }
    elapsed = task->frame - task->letterStart[i];
    letterX = x + (elapsed > SLIDE_FRAMES
                       ? 0
                       : Math_Ease(4, 256, 0, elapsed, SLIDE_FRAMES));
    if (letterX >= 256) {
      break;
    }
    if (task->glyphs[i] == MODE_FONT_SPACE) {
      x += 14;
    } else {
      Sprite_Queue(task->unk_5c, (letterX & 0x1ff) | LETTER_AFFINE_INDEX,
                   (y & 0xff) | OAM_AFFINE, &data_027c0000,
                   task->unk_56 | (task->paletteSlot << 12),
                   task->tile + task->glyphs[i] * 8);
      x += data_02159814[task->glyphs[i]] - 2;
    }
  }
  if (task->frame == 56) {
    task->frame = 0;
    Task_ResumeScript((Task*)task);
  }
}
