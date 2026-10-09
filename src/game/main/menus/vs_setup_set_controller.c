#include "types.h"
#include "game/menu_state.h"

void Sound_PlaySe(int id);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Sets the controller of entry event - 1 of the setup screen (event ids
 * 1-8 are the controller entries; the first change plays a sound). */
void VsSetup_SetController(int event, int controller)
{
    u8 index;

    if (MENU->setup.settingsChanged == 0 && MENU->setup.unk_2ea == 0) {
        Sound_PlaySe(0x10002);
        MENU->setup.settingsChanged = 1;
    }
    index = event - 1;
    MENU->setup.controllers[index >> 1][index & 1] = controller;
}
