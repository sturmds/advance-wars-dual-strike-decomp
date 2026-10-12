#include "types.h"
#include "game/Unk_ov006_0235f690.h"

int Ov006_SlotsAreShown(void);
void Ov006_PressMenuButton(int buttonId);
extern Unk_ov006_0235f690* data_ov006_0236d640;

/* Chooses slot button 38 once the slots are shown. */
void Ov006_ChooseButton38(void) {
  if (!Ov006_SlotsAreShown()) {
    return;
  }
  data_ov006_0236d640->chosenButton = -1;
  Ov006_PressMenuButton(38);
}
