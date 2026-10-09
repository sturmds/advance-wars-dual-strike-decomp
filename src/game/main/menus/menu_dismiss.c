#include "types.h"

void Task_ResumeByScript(const void *script);
void MenuButtons_CloseAll(void);
void *Task_Create(const void *script, void *parent);
extern u8 data_0215af10[];
extern u8 data_0215aed0[];

void Menu_Dismiss(void)
{
    Task_ResumeByScript(data_0215af10);
    MenuButtons_CloseAll();
    Task_Create(data_0215aed0, (void *)3);
}
