#include "types.h"
#include "game/Unk_ov006_0235f690.h"

extern Unk_ov006_0235f690* data_ov006_0236d640;

/* True when at least 7 of the 8 slots have finished sliding in (state 20). */
int Ov006_SlotsAreShown(void) {
  int i;
  int count = 0;

  for (i = 32; i <= 39; i++) {
    if (data_ov006_0236d640->unk_80[i - 32].state == 20) {
      count++;
    }
  }
  return count >= 7;
}
