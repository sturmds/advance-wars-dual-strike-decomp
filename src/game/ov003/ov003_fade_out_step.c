#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov003_0235dd38.h"
#include "game/ov003_unit.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(void* task);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Task step: darkens both screens over 48 frames (counter in unk_4c, held while byte 0x1d7 of the state is set), then resumes the task script. */
void Ov003_FadeOutStep(Unk_ov003_0235dd38* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* saved027c;
  int brightness;

  if (OV003_UNK_1D7(data_ov003_02364928) == 0) {
    task->unk_4c = (s16)task->unk_4c + 1;
  }
  brightness = Math_Ease(0, 0, -16, (s16)task->unk_4c, 48);
  savedGfx = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->masterBrightness = brightness;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->masterBrightness = brightness;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(savedGfx);
  if ((s16)task->unk_4c == 48) {
    task->unk_4c = 0;
    Task_ResumeScript(task);
  }
}
