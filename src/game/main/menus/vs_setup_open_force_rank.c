#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

BOOL Skills_AreDisabled(void);
int Co_GetRank(u32 co);
void Sound_PlaySe(int id);
void func_0206949c(int cursor);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* X on a CO entry of the Vs setup screen: starts func_0206949c for that CO
 * when it has a Force Rank to edit, else plays the refusal sound. */
void VsSetup_OpenForceRank(int unused)
{
    int cursor;

    switch (Menu_GetSelectedId()) {
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        cursor = Menu_GetSelectedId() - 8;
        if (Skills_AreDisabled() != 1 && MENU->setup.cos[cursor >> 1][cursor & 1] != 0
            && Co_GetRank(MENU->setup.cos[cursor >> 1][cursor & 1]) != 0) {
            Sound_PlaySe(0x10006);
            func_0206949c(cursor);
            return;
        }
        break;
    }
    Sound_PlaySe(0x10009);
}
