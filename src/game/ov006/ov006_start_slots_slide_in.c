#include "types.h"
#include "game/Unk_ov006_0235f690.h"

extern Unk_ov006_0235f690* data_ov006_0236d640;

/* Starts the slide-in of the eight slot buttons. */
void Ov006_StartSlotsSlideIn(void) {
  int i;

  for (i = OV006_SLOT_BUTTON_FIRST; i <= OV006_SLOT_BUTTON_LAST; i++) {
    data_ov006_0236d640->unk_80[i - OV006_SLOT_BUTTON_FIRST].state = 10;
  }
}
