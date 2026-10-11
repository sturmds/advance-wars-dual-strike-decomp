#include "types.h"
#include "game/menu_state.h"

void Sound_PlaySe(int id);
int Link_FindSlotOfPlayer(int player);
int Link_GetOwnPlayer(void);
void VsSetup_ResumePlayerTask(int player);
void VsSetup_AddLinkButtons(int player);
extern MenuState gMenuState;
#define MENU ((MenuState *)&gMenuState)

void VsSetup_ClearPlayerReady(int player)
{
    Sound_PlaySe(0x10009);
    MENU->setup.unk_2d0[player] = 0;
    VsSetup_ResumePlayerTask(player);
    if (player == Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1) {
        VsSetup_AddLinkButtons(player);
    }
}
