#include "types.h"
#include "game/map_unit.h"

extern u8 gAiTransportsSentToUnit[];

void Ai_ClearTransportsSentToUnit(void) {
  int i;

  for (i = 0; i < 64; i++) {
    gAiTransportsSentToUnit[i] = 0;
  }
}
