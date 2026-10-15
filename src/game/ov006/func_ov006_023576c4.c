#include "types.h"
#include "game/Unk_ov006_0235f690.h"

int func_ov006_023576c4(void) {
  Unk_ov006_0235f690* state = data_ov006_0236d640;

  if (state->request != 2) {
    return 0;
  }
  return state->screenState == 31;
}
