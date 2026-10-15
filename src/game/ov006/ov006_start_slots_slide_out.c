#include "types.h"
#include "game/Unk_ov006_0235f690.h"

/* Starts the slide-out of the eight slot buttons. */
void Ov006_StartSlotsSlideOut(void) {
  int i;

  for (i = OV006_SLOT_BUTTON_FIRST; i <= OV006_SLOT_BUTTON_LAST; i++) {
    data_ov006_0236d640->unk_80[i - OV006_SLOT_BUTTON_FIRST].state = 30;
  }
}
