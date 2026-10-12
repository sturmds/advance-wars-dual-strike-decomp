#include "types.h"
#include "game/Unk_ov006_0235f690.h"

extern Unk_ov006_0235f690* data_ov006_0236d640;

/* In request 6, state 1 or 11 moves on to state 30. Returns 1 if it did. */
int func_ov006_02358fec(void) {
  Unk_ov006_0235f690* state = data_ov006_0236d640;

  if (state->request != 6) {
    return 0;
  }
  if (state->screenState != 1 && state->screenState != 11) {
    return 0;
  }
  state->screenState = 30;
  return 1;
}
