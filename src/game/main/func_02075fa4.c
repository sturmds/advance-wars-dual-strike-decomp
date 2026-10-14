#include "types.h"
#include "game/task.h"

typedef struct Unk_02075fa4_0 Unk_02075fa4_0;

struct Unk_02075fa4_0 {
  u8 pad_00[0x52];
  u16 unk_52;
};

Unk_02075fa4_0* Task_FindByScript(void*);
extern TaskScriptCmd data_0215fc0c[];
int func_02075e08(int);

int func_02075fa4(void) {
  Unk_02075fa4_0* t0 = Task_FindByScript(data_0215fc0c);
  return func_02075e08(t0->unk_52);
}
