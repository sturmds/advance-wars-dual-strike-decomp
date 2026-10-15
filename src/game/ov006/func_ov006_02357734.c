#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void func_ov006_02357734(void) {
  Unk_ov006_0235f690* state = data_ov006_0236d640;

  if (state->request != 2) {
    return;
  }
  if (state->screenState == 0x33) {
    state->screenState = 0x3c;
  }
}
