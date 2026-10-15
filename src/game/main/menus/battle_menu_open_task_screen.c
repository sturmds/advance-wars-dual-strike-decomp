#include "types.h"
#include "game/Unk_02080440.h"

void PopupMenu_PushState(void);
void func_0201e954(void);
void func_020b9e04(void);
void func_02075448(void);

/* Handler of the first entry of gBattleMenuItems. Remembers menuArg (read back
 * as Unk_02080440_48.unk_10 when the battle menu opens again), saves the
 * pop-up menu state and starts a menu task (func_02075448). */
void BattleMenu_OpenTaskScreen(int unused, u32 menuArg) {
  data_027c027c->unk_48[data_027c0284]->unk_10 = menuArg;
  PopupMenu_PushState();
  func_0201e954();
  func_020b9e04();
  func_02075448();
}
