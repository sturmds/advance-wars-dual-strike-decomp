#include "types.h"
#include "game/Unk_02290a60.h"

void func_0210408c(void) {
  int i;
  Unk_02290a60* record = data_02290a60;

  for (i = 0; i < 3; i++, record++) {
    record->unk_00 = 0;
    record->unk_01 = 0;
    record->unk_02 = 0;
    record->unk_04 = 0;
    record->unk_06 = 0;
    record->unk_03 = 0;
    record->unk_05 = 0;
    record->unk_08 = 0;
    record->unk_0a = 0;
    record->unk_0c = 0;
    record->unk_0e = 0;
  }
}
