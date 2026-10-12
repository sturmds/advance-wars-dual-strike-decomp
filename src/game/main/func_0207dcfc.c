#include "types.h"

typedef struct Unk_0229fb30 Unk_0229fb30;

struct Unk_0229fb30 {
  u8 pad_00[0x630];
  u8 unk_630;
};

extern Unk_0229fb30 data_0229fb30;
void MenuButtons_CloseAll(void*, int);

void func_0207dcfc(void) {
  data_0229fb30.unk_630 = 0;
  MenuButtons_CloseAll(&data_0229fb30, 0);
}
