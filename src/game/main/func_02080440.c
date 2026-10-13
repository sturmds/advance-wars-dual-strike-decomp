#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_action.h"

void func_020800c0(void);
int AiTurn_GetState(void);
void AiTurn_ShowAction(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
/* also declared in src/game/main/ai/ai_show_action_step0.c and 3 more */
u16 data_0229d8c4;
UnitAction data_0229d8c8;

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
