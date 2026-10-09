#include "types.h"
#include "game/menu_button.h"

void *Task_Create(const void *script, void *parentOrList);
extern const u32 data_0215d62c[];

void func_02070824(void)
{
    MenuButtons_RemoveSelectable();
    MenuButtons_DisableTouch();
    Task_Create(data_0215d62c, (void *)3);
}
