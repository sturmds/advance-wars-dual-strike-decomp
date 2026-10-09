#include "types.h"
#include "game/menu_button.h"

void *Task_Create(const void *script, void *parentOrList);

extern const u32 data_0215aa90[];

void CoSelectScreen_BeginExit(void)
{
    MenuButtons_RemoveSelectable();
    MenuButtons_DisableTouch();
    Task_Create(data_0215aa90, (void *)3);
}
