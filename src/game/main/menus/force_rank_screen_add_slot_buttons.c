#include "types.h"
#include "game/force_rank_screen.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_0215a7cc;
extern const MenuButtonTemplate data_0215a7f4;
extern const MenuButtonTemplate data_0215a81c;
extern const MenuButtonTemplate gForceRankSlotButtonTemplate;

/* Button ids 0xf0 to 0xf3 are the four skill slots. */
#define SLOT_BUTTON_ID(slot) (0xf0 + (slot))

/* Adds the buttons for the chosen-skill slots: one per slot that the Force
 * Rank opens (rank is the rank; at x 160, 16 pixels apart), plus buttons
 * 0xfc and 0xfd from rank 2 on, and button 0xfe. Touch input is turned off. */
void ForceRankScreen_AddSlotButtons(void) {
  int i;

  if (gForceRankScreen->rank >= 2) {
    MenuButton_Add(0, 0, &data_0215a81c, 0xfd);
    MenuButton_Add(0, 0, &data_0215a7cc, 0xfc);
  }
  for (i = 0; i < FORCE_RANK_SLOT_COUNT; i++) {
    if (gForceRankScreen->rank <= i) {
      break;
    }
    MenuButton_Add(160 + i * 16, 8, &gForceRankSlotButtonTemplate,
                   SLOT_BUTTON_ID(i));
  }
  MenuButton_Add(0, 0, &data_0215a7f4, 0xfe);
  MenuButtons_DisableTouch();
}
