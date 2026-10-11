#include "types.h"
#include "game/menu_item.h"

extern MenuItem data_02165aec[];
void ScreenConfig_RunActionHook(void);
void Font_LoadDigits(int baseTile);
void func_020c4018(void);
int func_020bc6b4(void);
int func_020bc6ac(void);
void func_0201e980(MenuItem *items, int x, int y, int arg, int arg2);
void MenuDepth_Push(void);

void func_020bc5c0(void)
{
    int x;

    ScreenConfig_RunActionHook();
    Font_LoadDigits(0);
    func_020c4018();
    x = func_020bc6b4();
    func_0201e980(data_02165aec, x, func_020bc6ac(), 1, 0);
    MenuDepth_Push();
}
