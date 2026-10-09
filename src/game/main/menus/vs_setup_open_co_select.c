#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

void *Task_Create(const void *script, void *parent);

extern MenuState gMenuState;
extern u8 data_0215ce68[];

/* Puts the setup cursor (ids 8-15 are COs) on setup.unk_2e5's front CO and
 * starts a task. The task's purpose is a guess. */
void *VsSetup_OpenCoSelect(void)
{
    MenuButtons_RemoveSelectable();
    MenuButtons_DisableTouch();
    gMenuState.setup.unk_2e4 = gMenuState.setup.unk_2e5 * 2 + 8;
    return Task_Create(data_0215ce68, (void *)3);
}
