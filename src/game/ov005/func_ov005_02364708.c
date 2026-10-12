#include "types.h"

typedef struct Unk_02364708 Unk_02364708;

struct Unk_02364708 {
  u8 pad_00[0x48];
  int unk_48;
};

void Task_Destroy(int);

void func_ov005_02364708(Unk_02364708* a0) { Task_Destroy(a0->unk_48); }
