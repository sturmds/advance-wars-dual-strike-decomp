#include "types.h"

typedef struct Unk_02038164_0 Unk_02038164_0;

struct Unk_02038164_0 {
  u8 pad_00[0xa4];
  int unk_a4;
};

extern Unk_02038164_0* data_0229c734;

int func_02038164(void) { return data_0229c734->unk_a4; }
