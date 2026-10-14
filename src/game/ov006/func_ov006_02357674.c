#include "types.h"
#include "game/Unk_ov006_0235f690.h"

extern Unk_ov006_0235f690* data_ov006_0236d640;

int func_ov006_02357674(void) {
  Unk_ov006_0235f690* t0 = data_ov006_0236d640;
  u16 t1 = t0->screenState;
  if (t1 == 51) {
    t0->screenState = 30;
    return 30;
  }
  return t1;
}
