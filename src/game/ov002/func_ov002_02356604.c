#include "types.h"

void BattleSpriteTask_Create(int a0, int a1, int a2, int a3, int a4, int a5,
                             int a6, int a7, int a8, int a9, int a10, int a11,
                             int a12, int a13, int a14, int a15, int a16,
                             int a17, int a18);

void func_ov002_02356604(int a0, int a1, int a2, int a3, int a4, int a5, int a6,
                         int a7, int a8, int a9, int a10, int a11, int a12,
                         int a13) {
  BattleSpriteTask_Create(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12,
                          a13, 0, 0, 0, 0, a11);
}
