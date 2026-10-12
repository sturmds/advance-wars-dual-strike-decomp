#include "types.h"
#include "game/keypad.h"
#include "game/touch.h"
#include "game/Unk_02160ac0.h"
#include "game/ov003_unit.h"

void func_0207b4b0(Ov003Input* input);
extern Ov003Input data_ov003_02365b04;

void Ov003_ReadInput(void) {
  data_ov003_02365b04.phase = gTouch.phase;
  data_ov003_02365b04.active = gTouch.active;
  data_ov003_02365b04.x = gTouch.x;
  data_ov003_02365b04.y = gTouch.y;
  data_ov003_02365b04.keys = gKeypad->held;
  if (data_02160ac0->unk_7c != 0) {
    func_0207b4b0(&data_ov003_02365b04);
  }
}
