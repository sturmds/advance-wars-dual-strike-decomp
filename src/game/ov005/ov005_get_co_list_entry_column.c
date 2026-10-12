#include "types.h"

extern u8 data_ov005_0236a5a2[][4];

u8 Ov005_GetCoListEntryColumn(int index) {
  return data_ov005_0236a5a2[index][0];
}
