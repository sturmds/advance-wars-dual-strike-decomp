#include "types.h"
#include "game/menu_button.h"

void *Task_Create(const void *script, void *parentOrList);
extern const u32 data_0215a960[];

void func_02042b3c(void)
{
    MenuButtons_RemoveSelectable();
    MenuButtons_DisableTouch();
    Task_Create(data_0215a960, (void *)3);
}
