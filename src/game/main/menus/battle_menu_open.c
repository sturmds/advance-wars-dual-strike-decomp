#include "types.h"
#include "game/Unk_02080440.h"
#include "game/menu_item.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void ScreenConfig_RunActionHook(void);
void Font_LoadDigits(int baseTile);
void func_020c4018(void);
int func_020bc6b4(void);
int func_020bc6ac(void);
void PopupMenu_Create(MenuItem* items, int a, int b, int c, int d, int e);
void MenuDepth_Push(void);

/* Builds the battle map menu (gBattleMenuItems). */
void BattleMenu_Open(void) {
  int a;

  ScreenConfig_RunActionHook();
  Font_LoadDigits(0);
  func_020c4018();
  a = func_020bc6b4();
  PopupMenu_Create(gBattleMenuItems, a, func_020bc6ac(), 1,
                   data_027c027c->unk_48[data_027c0284]->unk_10, 0);
  MenuDepth_Push();
}
