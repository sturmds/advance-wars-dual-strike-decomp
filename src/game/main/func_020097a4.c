#include "types.h"

void Screen_ResetBgRegisters(void);
void Screen_ResetBlend(void);
void Screen_ResetWindows(void);
void func_02003ff8(void);

void func_020097a4(void)
{
    Screen_ResetBgRegisters();
    Screen_ResetBlend();
    Screen_ResetWindows();
    func_02003ff8();
}
