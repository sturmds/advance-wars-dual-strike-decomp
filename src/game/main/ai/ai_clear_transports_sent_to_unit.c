#include "types.h"
#include "game/map_unit.h"

extern u8 gAiTransportsSentToUnit[];

void Ai_ClearTransportsSentToUnit(void) {
  int i;

  for (i = 0; i < MAP_UNITS_PER_PLAYER; i++) {
    gAiTransportsSentToUnit[i] = 0;
  }
}
