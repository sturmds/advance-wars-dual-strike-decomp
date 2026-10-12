#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
void func_0201e954(void);
void func_020bc548(void);

void func_020c02f0(int unused, u32 value) {
  data_027c027c->unk_48[data_027c0284]->unk_14 = value;
  func_0201e954();
  func_020bc548();
}
