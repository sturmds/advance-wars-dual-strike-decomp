/* Built with opt_loop_invariants off (the pragma sweep found it): the original
 * reads playerCount again on every pass of the loop. */
#pragma opt_loop_invariants off
#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_state.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void PaletteCycle_Stop(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
/* also declared in src/game/main/link/link_assign_slots.c and 4 more */
u8 gLinkSlotPlayers[4];

/* Step of the link screen's closing task script: stops the palette cycle on
 * the current screen and on screen data_027c0288.unk_04, then stores each
 * panel's player + 1 into gLinkSlotPlayers. */
void LinkScreen_SavePanelOrder(void) {
  /* Needed to match: saved027c declared after i. */
  Unk_027c0280* saved0280;
  int i;
  Unk_02080440* saved027c;

  PaletteCycle_Stop();
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  PaletteCycle_Stop();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  for (i = 0; i < gMenuState.link.playerCount; i++) {
    gLinkSlotPlayers[i] = gMenuState.link.panelPlayers[i] + 1;
  }
}
