#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;

void func_020f2190(void);
void func_020f21e8(int value);
void func_020f2210(int value);
void func_020f4990(int arg);

void func_020f3604(int value, int target) {
  func_020f2190();
  func_020f2210(value);
  func_020f21e8(value);
  data_021694c0->unk_3c = 0;
  data_021694c0->unk_38 = target;
  data_021694c0->unk_14 = value;
  func_020f4990(5);
}
