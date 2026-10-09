#include "types.h"
#include "game/menu_button.h"

void *Task_Create(const void *script, void *parentOrList);
extern const u32 data_0215d400[];

void func_0206ef54(void)
{
    MenuButtons_DisableTouch();
    MenuButtons_CloseAll();
    Task_Create(data_0215d400, (void *)3);
}
