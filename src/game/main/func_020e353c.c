#include "types.h"

typedef struct Unk_02168850Row {
  u32 unk_00;
  u32 unk_04;
  u32 unk_08;
} Unk_02168850Row;
extern Unk_02168850Row data_02168854[];

u32 func_020e353c(int index) { return data_02168854[index].unk_00; }
