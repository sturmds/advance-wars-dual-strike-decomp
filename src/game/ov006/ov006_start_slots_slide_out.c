#include "types.h"
#include "game/Unk_ov006_0235f690.h"

extern Unk_ov006_0235f690* data_ov006_0236d640;

/* Starts the slide-out of the eight slot buttons. */
void Ov006_StartSlotsSlideOut(void) {
  int i;

  for (i = 32; i <= 39; i++) {
    data_ov006_0236d640->unk_80[i - 32].state = 30;
  }
}
