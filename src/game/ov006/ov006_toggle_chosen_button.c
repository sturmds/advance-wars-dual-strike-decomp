#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_ov006_0235f690.h"

extern Unk_ov006_0235f690* data_ov006_0236d640;

/* Gives slot buttons 33 to 39 their normal tile, then toggles chosenId. The
 * chosen button gets the highlighted tile, which lies 7 button tiles on. */
void Ov006_ToggleChosenButton(int chosenId) {
  int id;
  MenuButton* button;
  int index;

  for (id = 33; id <= 39; id++) {
    button = MenuButton_Find(id);
    index = button->id - 32;
    button->tile = 0x25e + index * 16;
    if (button->id == chosenId) {
      if (data_ov006_0236d640->chosenButton != chosenId) {
        button->tile = 0x25e + (index + 7) * 16;
        data_ov006_0236d640->chosenButton = chosenId;
      } else {
        data_ov006_0236d640->chosenButton = -1;
      }
    }
  }
}
