#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

void Sound_PlaySe(int id);
int Link_IsActive(void);
int Link_GetOwnPlayer(void);
int Link_FindSlotOfPlayer(int player);
void Co_QueueArmyMediumPicture(int co, void *dest, int paletteSlot, int player);
void VsSetup_RedrawSkills(void);
void VsSetup_RedrawAllSkills(void);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Sets the CO of entry event - 9 of the setup screen, loads its picture and
 * redraws the skills. In a link game only the entry of this machine's own
 * player is redrawn. Plays SE 0x10002, at most once a frame (cursorSePlayed). */
void VsSetup_SetCo(int event, int co)
{
    u8 index;
    int player;

    if (MENU->setup.cursorSePlayed == 0 && MENU->setup.unk_2ea == 0) {
        Sound_PlaySe(0x10002);
        MENU->setup.cursorSePlayed = 1;
    }
    index = event - 9;
    player = index >> 1;
    MENU->setup.cos[player][index & 1] = co;
    if (MENU->setup.cos[player][index & 1] != 0 && MENU->setup.unk_2ea == 0) {
        Co_QueueArmyMediumPicture(MENU->setup.cos[player][index & 1], data_027c0280->objVram + (552 + index * 18) * 32, index + 18, player + 1);
    }
    /* Needed to match: the two calls are written out in both branches. */
    if (Link_IsActive() == 1) {
        if (player != Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1) {
            return;
        }
        VsSetup_RedrawSkills();
        VsSetup_RedrawAllSkills();
    } else {
        VsSetup_RedrawSkills();
        VsSetup_RedrawAllSkills();
    }
}
