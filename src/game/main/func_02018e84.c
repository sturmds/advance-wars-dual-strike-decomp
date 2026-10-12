#include "types.h"

typedef struct PlayRecords PlayRecords;

struct PlayRecords {
  u8 pad_00[0xf];
  u8 unk_f;
};

extern PlayRecords gPlayRecords;

int func_02018e84(void) { return (gPlayRecords.unk_f == 0) ? 1 : 0; }
