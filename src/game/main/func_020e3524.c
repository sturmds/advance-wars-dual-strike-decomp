#include "types.h"

typedef struct Unk_02168850Row {
  u32 unk_00;
  u32 unk_04;
  u32 unk_08;
} Unk_02168850Row;

extern Unk_02168850Row data_02168858[];

u32 func_020e3524(int index) { return data_02168858[index].unk_00; }
