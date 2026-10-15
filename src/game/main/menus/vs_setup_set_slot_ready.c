#include "types.h"
#include "game/menu_state.h"

int Sound_PlaySe(int);
int Link_FindSlotOfPlayer(int player);
int VsSetup_GetSlotTileX(int player);
void VsSetup_StartPlayerTask(int slot, u16 x, int y);
int Link_GetOwnPlayer(void);
void VsSetup_CloseTeamArrowButtons(int slot);

/* Marks a slot ready on the team screen (once): plays a sound, sets its flag
 * in setup.ready and calls VsSetup_StartPlayerTask at the slot position, y 72 (a
 * ready marker, a guess). On the own slot of this machine it also removes
 * the team arrow buttons (VsSetup_CloseTeamArrowButtons). Shifted view: see VsSetup_NextTeam. */
void VsSetup_SetSlotReady(int slot) {
  MenuState* shifted = (MenuState*)((u8*)&gMenuState + slot);

  if (shifted->setup.ready[0] != 0) {
    return;
  }
  Sound_PlaySe(0x10006);
  shifted->setup.ready[0] = 1;
  VsSetup_StartPlayerTask(slot, VsSetup_GetSlotTileX(slot) * 8 + 16, 72);
  if (slot != Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1) {
    return;
  }
  VsSetup_CloseTeamArrowButtons(slot);
}
