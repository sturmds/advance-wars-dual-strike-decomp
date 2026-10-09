#include "types.h"
#include "game/menu_button.h"

void *Task_Create(const void *script, void *parentOrList);
extern const u32 data_0215c968[];

void func_0205f538(void)
{
    MenuButtons_RemoveSelectable();
    MenuButtons_DisableTouch();
    Task_Create(data_0215c968, (void *)3);
}
