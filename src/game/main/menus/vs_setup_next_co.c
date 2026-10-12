#include "types.h"
#include "game/menu_state.h"

int VsSetup_CanPickCo(int coSlot, int co);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Like VsSetup_PrevCo, but the CO after the one in coSlot. */
int VsSetup_NextCo(int coSlot) {
  u8 co = MENU->setup.cos[coSlot >> 1][coSlot & 1];
  u8* list = MENU->setup.coList;
  int i = 0;

  while (co != list[i]) {
    i++;
  }
  do {
    i++;
    if (i == MENU->setup.coListCount) {
      i = 0;
    }
  } while (VsSetup_CanPickCo(coSlot, MENU->setup.coList[i]) == 0);
  return MENU->setup.coList[i];
}
