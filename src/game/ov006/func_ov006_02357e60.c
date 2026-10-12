#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void func_ov006_02357eac(int id);

extern Unk_ov006_0235f690* data_ov006_0236d640;

void func_ov006_02357e60(int unused0, int unused1, int mode) {
  if (!(data_ov006_0236d640->unk_00 & 0x200)) {
    return;
  }
  if (mode == 2) {
    return;
  }
  func_ov006_02357eac(0);
}
