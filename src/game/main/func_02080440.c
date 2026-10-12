#include "types.h"
#include "game/Unk_02080440.h"

void func_020800c0(void);
int AiTurn_GetState(void);
void AiTurn_ShowAction(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 data_0229d8c4;

void func_02080440(void) {
  func_020800c0();
  if ((data_027c027c->unk_48[data_027c0284]->unk_1c4->scrollFlags & 0xf) != 0) {
    return;
  }
  AiTurn_ShowAction();
  if (AiTurn_GetState() == 2) {
    data_0229d8c4 = 0;
  }
}
