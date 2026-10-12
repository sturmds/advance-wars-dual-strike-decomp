#include "types.h"

extern u8 gAiTransportsSentToUnit[64];

void Ai_ClearTransportsSentToUnit(void) {
  int i;

  for (i = 0; i < 64; i++) {
    gAiTransportsSentToUnit[i] = 0;
  }
}
