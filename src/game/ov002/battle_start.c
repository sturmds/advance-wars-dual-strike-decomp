#include "types.h"

void func_ov002_023511ac(void);
void Battle_InitScreenWindows(void);
int func_ov002_0235059c(void);
void func_ov002_02350e1c(void);
void Battle_DrawFrame(void);
void func_01ff9c48(const void *descriptor);

void Battle_StartScreen(void)
{
    func_ov002_023511ac();
    Battle_InitScreenWindows();
    func_ov002_0235059c();
    func_ov002_02350e1c();
    func_01ff9c48(Battle_DrawFrame);
}
