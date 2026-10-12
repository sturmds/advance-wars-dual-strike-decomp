#include "types.h"

void Ov006_UpdateEntryView(int index);

void func_ov006_023564b0(const u8* value) {
  Ov006_UpdateEntryView(*value - 64);
}
