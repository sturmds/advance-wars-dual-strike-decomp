#include "types.h"
#include "game/Unk_02080440.h"
#include "game/menu_item.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

extern MenuItem data_02165d2c[];

void ScreenConfig_RunActionHook(void);
void Font_LoadDigits(int baseTile);
void func_020c4018(void);
int func_020bc6b4(void);
int func_020bc6ac(void);
void PopupMenu_Create(MenuItem* items, int a, int b, int c, int d, int e);
void MenuDepth_Push(void);

/* Builds a pop-up menu from the table data_02165d2c, as BattleMenu_Open does. */
void func_020bc4cc(void) {
  int a;

  ScreenConfig_RunActionHook();
  Font_LoadDigits(0);
  func_020c4018();
  a = func_020bc6b4();
  PopupMenu_Create(data_02165d2c, a, func_020bc6ac(), 1,
                   data_027c027c->unk_48[data_027c0284]->unk_14, 0);
  MenuDepth_Push();
}
