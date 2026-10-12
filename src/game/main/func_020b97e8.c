#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void NumberFont_LoadLow(void);
void Battle_RegisterFrameCallbacks(void);
void func_020c4018(void);

#define HOOKS (data_027c027c->unk_50[data_027c0284])

void func_020b97e8(void) {
  if (HOOKS->unk_54 != NULL) {
    HOOKS->unk_54(1);
  }
  NumberFont_LoadLow();
  Battle_RegisterFrameCallbacks();
  func_020c4018();
}
