#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_ov003_0235dd38.h"
#include "game/math.h"
#include "game/oam_buffer.h"
#include "game/win_quote.h"

/* Needed to match: const, so the sine reads move out of the loop. */

void BreakText_QueueLetterSized(u8 letter, u16 x, int y, int arg3, int list);
s16 BreakText_GetLetterX(u8 letter);
void Task_ResumeScript(Unk_ov003_0235dd38* task);

/* High byte of Ov003State.unk_1d6. */
#define STATE_BYTE_1D7 (((u8*)&data_ov003_02364928->unk_1d6)[1])

/* Needed to match: Task.unk_4c is read as signed. */
#define STEP(task) ((s16)(task)->unk_4c)

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Script command that runs for 3 * count + 8 steps, then resumes the
 * script (see Ov003_StepCoPowerBanner for the fields). Task.slideX eases from
 * 0 to -240 and moves the banner sprite. Letter i is drawn at rest until step
 * 3 * i; for the next 8 steps it is drawn through affine matrix i + 1, which
 * grows it from its size to double; then it is not drawn. Needed to match: the
 * sine products are read from the table in separate statements. */
void Ov003_StepLettersGrow(Unk_ov003_0235dd38* task) {
  int i;
  int scale;

  if (STATE_BYTE_1D7 == 0) {
    task->unk_4c = STEP(task) + 1;
  }
  task->slideX = Math_Ease(1, 0, -240, STEP(task), task->count * 3 + 8);
  Sprite_QueueCoPortrait((task->slideX + 184) & 0x1ff, 160, 0x3000, 0x58e, 3,
                         0);
  for (i = 0; i < task->count; i++) {
    if (STEP(task) < i * 3) {
      BreakText_QueueLetterSized(i, BreakText_GetLetterX(i) & 0x1ff, 80, 0, 2);
    } else if (STEP(task) >= i * 3 && STEP(task) < i * 3 + 8) {
      scale = Math_Ease(1, 256, 512, STEP(task) - i * 3, 8);
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
          i, (BreakText_GetLetterX(i) & 0x1ff) | ((i + 1) << 9), 0x350, 0, 2);
    }
  }
  if (STEP(task) == task->count * 3 + 8) {
    task->unk_4c = 0;
    Task_ResumeScript(task);
  }
}
