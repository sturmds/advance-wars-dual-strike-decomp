#include "types.h"
#include "game/menu_state.h"

void Sound_PlaySe(int id);
int Link_FindSlotOfPlayer(int player);
int Link_GetOwnPlayer(void);
void VsSetup_ResumePlayerTask(int player);
void VsSetup_AddLinkSlotButtons(int player);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

void VsSetup_ClearSlotReady(int player) {
  Sound_PlaySe(0x10009);
  MENU->setup.ready[player] = 0;
  VsSetup_ResumePlayerTask(player);
  if (player == Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1) {
    VsSetup_AddLinkSlotButtons(player);
  }
}
