#include "types.h"
#include "game/menu_state.h"

int VsSetup_CanPickCo(int coSlot, int co);

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* The CO before the one in coSlot in the pick list (wrapping), skipping COs
 * VsSetup_CanPickCo refuses for that slot. */
int VsSetup_PrevCo(int coSlot) {
  u8 co = MENU->setup.cos[coSlot >> 1][coSlot & 1];
  u8* list = MENU->setup.coList;
  int i = 0;

  while (co != list[i]) {
    i++;
  }
  do {
    i--;
    if (i < 0) {
      i = MENU->setup.coListCount - 1;
    }
  } while (VsSetup_CanPickCo(coSlot, MENU->setup.coList[i]) == 0);
  return MENU->setup.coList[i];
}
