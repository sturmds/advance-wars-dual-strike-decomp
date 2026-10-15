#include "types.h"
#include "game/task.h"
#include "game/math.h"
#include "game/oam_buffer.h"

/* Needed to match: const, so the sine reads move out of the loop. */

void TagBreak_DrawSidePiece(int x, int y, int side);
s16 BreakText_GetScreenX(u8 slot);
void BreakText_QueueLetter(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)
/* Slide offset kept in the task's free bytes at 0x2c. */
#define SLIDE(task) (*(int*)(task)->pad_2c)

/* Divisor for one matrix entry: the eased height, or 2 if it is 0. */
#define DIVISOR(height) ((height) ? (height) : 2)

#define STAGGER_FRAMES 4 /* between the starts of two letters */
#define STRETCH_FRAMES 24

/* Step of the Tag Power cut-in script. The two side pieces slide in from 256
 * pixels out. Every STAGGER_FRAMES frames one more letter of the name starts
 * to unfold: it is drawn through affine matrix i + 1, squashed to 1/16 of its
 * height, and grows to full height over STRETCH_FRAMES frames. */
void TagPowerScreen_StretchInName(Task* task) {
  int i;

  FRAME(task)++;
  SLIDE(task) = Math_Ease(4, 256, 0, FRAME(task),
                          task->unk_58 * STAGGER_FRAMES + STRETCH_FRAMES);
  TagBreak_DrawSidePiece(SLIDE(task) + 72, 224, 0);
  TagBreak_DrawSidePiece(184 - SLIDE(task), 224, 1);
  for (i = 0; i < task->unk_58; i++) {
    if (FRAME(task) < i * STAGGER_FRAMES) {
      continue;
    }
    if (FRAME(task) < i * STAGGER_FRAMES + STRETCH_FRAMES) {
      int height;

      height = Math_Ease(4, 16, 256, FRAME(task) - i * STAGGER_FRAMES,
                         STRETCH_FRAMES);
      {
        int d3 = DIVISOR(height);
        int d1 = DIVISOR(height);
        int cosValue = gSineTable[64] * 16;
        int sinPos = gSineTable[0] * 16;
        int sinNeg = -gSineTable[0] * 16;
        Oam_SetAffineParams(i + 1, HwDiv32(cosValue, 256), HwDiv32(sinNeg, d1),
                            HwDiv32(sinPos, 256), HwDiv32(cosValue, d3));
      }
      BreakText_QueueLetter(
          i, (BreakText_GetScreenX(i) & 0x1ff) | ((i + 1) << 9), 0x150, 0, 0);
    } else {
      BreakText_QueueLetter(i, BreakText_GetScreenX(i) & 0x1ff, 80, 0, 0);
    }
  }
  if (FRAME(task) == task->unk_58 * STAGGER_FRAMES + STRETCH_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
