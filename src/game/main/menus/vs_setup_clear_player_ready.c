#include "types.h"
#include "game/menu_state.h"

void Sound_PlaySe(int id);
int Link_FindSlotOfPlayer(int player);
int Link_GetOwnPlayer(void);
void VsSetup_ResumePlayerTask(int player);
void VsSetup_AddLinkButtons(int player);
#define MENU ((MenuState*)&gMenuState)

/* Takes back the ready state of a player slot on the Vs setup screen, with
 * a sound, and resumes its panel task. For this machine's own slot it adds
 * the slot's buttons again. */
void VsSetup_ClearPlayerReady(int player) {
  Sound_PlaySe(0x10009);
  MENU->setup.ready[player] = 0;
  VsSetup_ResumePlayerTask(player);
  if (player == Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1) {
    VsSetup_AddLinkButtons(player);
  }
}
