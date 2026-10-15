#include "types.h"
#include "game/task.h"
#include "game/math.h"
#include "game/oam_buffer.h"
#include "game/win_quote.h"

/* Needed to match: const, so the sine reads move out of the loop. */

void Sound_PlaySe(u32 soundId);
s16 BreakText_GetCentredX(u8 slot);
void BreakText_QueueLetterSized(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)
/* Slide offset kept in the task's free bytes at 0x2c. */
#define SLIDE(task) (*(int*)(task)->pad_2c)

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

#define STAGGER_FRAMES 3 /* between the starts of two letters */
#define GROW_FRAMES 8

/* Step of the Super CO Power cut-in script. Every STAGGER_FRAMES frames one
 * more letter of the power name appears with sound effect 186, drawn through
 * affine matrix i + 1 at double size and easing back to normal over
 * GROW_FRAMES frames. SLIDE eases from 128 to 0 over the whole step. */
void SuperBreak_LettersPopInUpdate(Task* task) {
  int i;

  FRAME(task)++;
  SLIDE(task) = Math_Ease(4, 128, 0, FRAME(task),
                          task->unk_58 * STAGGER_FRAMES + GROW_FRAMES);
  Sprite_QueueCoPortrait(184, 160, 0x800, 16, 2, 0);
  for (i = 0; i < task->unk_58; i++) {
    if (FRAME(task) >= i * STAGGER_FRAMES &&
        FRAME(task) < i * STAGGER_FRAMES + GROW_FRAMES) {
      int scale;

      if (FRAME(task) - i * STAGGER_FRAMES == 0) {
        Sound_PlaySe(186);
      }
      scale =
          Math_Ease(4, 512, 256, FRAME(task) - i * STAGGER_FRAMES, GROW_FRAMES);
      {
        int d3 = DIVISOR(scale);
        int d2 = DIVISOR(scale);
        int d1 = DIVISOR(scale);
        int d0 = DIVISOR(scale);
        int cosValue = gSineTable[64] * 16;
        int sinPos = gSineTable[0] * 16;
        int sinNeg = -gSineTable[0] * 16;
        Oam_SetAffineParams(i + 1, HwDiv32(cosValue, d0), HwDiv32(sinNeg, d1),
                            HwDiv32(sinPos, d2), HwDiv32(cosValue, d3));
      }
      BreakText_QueueLetterSized(
          i, (BreakText_GetCentredX(i) & 0x1ff) | ((i + 1) << 9), 0x350, 0, 1);
    } else if (FRAME(task) >= i * STAGGER_FRAMES + GROW_FRAMES) {
      BreakText_QueueLetterSized(i, BreakText_GetCentredX(i) & 0x1ff, 80, 0, 1);
    }
  }
  if (FRAME(task) == task->unk_58 * STAGGER_FRAMES + GROW_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
