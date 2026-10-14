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
extern OamCellList1 data_027c0000; /* cell table of one 16x32 sprite */

#define SIN0 (gSineTable[0])
#define COS0 (gSineTable[64])

/* Divides v by the eased scale s, or by 2 if s is 0. */
#define DIV_SCALE(v, s) HwDiv32(v, (s) != 0 ? (s) : 2)

#define GROW_FRAMES 16

/* Affine parameter set the letters use, and its OAM attribute 1 bits. */
#define LETTER_AFFINE 31
#define LETTER_AFFINE_INDEX (LETTER_AFFINE << 9)

/* OAM attribute 0 bit: affine sprite. */
#define OAM_AFFINE 0x100

/* Task step, 16 frames: draws the line of mode font letters through one
 * affine matrix whose height eases from a quarter to full, then resumes
 * the script. */
void ModeFontTask_GrowIn(ModeFontTask* task) {
  int i;
  u16 x = task->x;
  u16 y = task->y;

  task->frame++;
  Oam_SetAffineParams(
      LETTER_AFFINE, HwDiv32(COS0 << 4, 256),
      DIV_SCALE(-SIN0 << 4, Math_Ease(0, 64, 256, task->frame, GROW_FRAMES)),
      HwDiv32(SIN0 << 4, 256),
      DIV_SCALE(COS0 << 4, Math_Ease(0, 64, 256, task->frame, GROW_FRAMES)));
  /* unused here; ModeFontTask_GrowInFramed slides its end sprites by this */
  Math_Ease(4, 128, 0, task->frame, GROW_FRAMES);
  for (i = 0; i < task->length; i++) {
    if (task->glyphs[i] == MODE_FONT_SPACE) {
      x += 14;
    } else {
      Sprite_Queue(task->unk_5c, (x & 0x1ff) | LETTER_AFFINE_INDEX,
                   (y & 0xff) | OAM_AFFINE, &data_027c0000,
                   task->unk_56 | (task->paletteSlot << 12),
                   task->tile + task->glyphs[i] * 8);
      x += data_02159814[task->glyphs[i]] - 2;
    }
  }
  if (task->frame == GROW_FRAMES) {
    task->frame = 0;
    Task_ResumeScript((Task*)task);
  }
}
