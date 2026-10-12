#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void Sound_PlaySe(u32 soundId);

extern Unk_ov006_0235f690* data_ov006_0236d640;

/* In request 6, state 1 moves to state 10 and state 11 to state 0, each with a
 * sound effect. Returns 1 if it did. */
int func_ov006_02359030(void) {
  Unk_ov006_0235f690* state = data_ov006_0236d640;

  if (state->request != 6) {
    return 0;
  }
  if (state->screenState == 1) {
    state->screenState = 10;
    Sound_PlaySe(0x10002);
    return 1;
  }
  if (state->screenState != 11) {
    return 0;
  }
  state->screenState = 0;
  Sound_PlaySe(0x10002);
  return 1;
}
