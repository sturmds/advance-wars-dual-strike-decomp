#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov003_0235dd38.h"
#include "game/ov003_unit.h"
#include "game/math.h"

void Task_ResumeScript(void* task);

/* Task step: while Ov003State.unk_1d7 is 0, counts 8 frames and eases the
 * blend coefficient EVY from 8 to 0, then resumes the task script. */
void Ov003_FadeEvyStep(Unk_ov003_0235dd38* obj) {
  if (data_ov003_02364928->unk_1d7 == 0) {
    obj->frame++;
  }
  data_027c0280->blendEvy = Math_Ease(0, 8, 0, obj->frame, 8);
  if (obj->frame == 8) {
    obj->frame = 0;
    Task_ResumeScript(obj);
  }
}
