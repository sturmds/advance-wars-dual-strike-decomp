#include "types.h"

extern int data_022a7e68;
void Task_ResumeScript(void *task);
void HelpBand_Start(int textId, int arg);
void HelpText_Show(u32 textId, int arg);
void MenuButtons_DisableTouch(void);
int ScreenTransition_IsRunning(void);
void LinkBanner_CreateWithText(void *task, u32 textId);
void func_020fc178(void);
void func_020fc234(void);
int func_020fc89c(void);

void func_020fc2fc(void *task)
{
    switch (data_022a7e68) {
    case 1:
        if (func_020fc89c() == 0) {
            data_022a7e68 = 5;
            return;
        }
        Task_ResumeScript(task);
        HelpBand_Start(0x7f8, 1);
        HelpText_Show(0x48000007, 0x3c0);
        func_020fc178();
        if (ScreenTransition_IsRunning() == 1) {
            MenuButtons_DisableTouch();
        }
        break;
    case 5:
        LinkBanner_CreateWithText(task, 0x48000002);
        func_020fc234();
        Task_ResumeScript(task);
        break;
    }
}
