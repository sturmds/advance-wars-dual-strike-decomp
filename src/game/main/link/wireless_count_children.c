#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/link_buffers.h"

/* Counts the connected children (aids 1 to 7). */
void Wireless_CountChildren(void) {
  int aid;

  data_0215fe84->childCount = 0;
  for (aid = 1; aid < LINK_AID_COUNT; aid++) {
    if (data_0215fe84->childAids & (1 << aid)) {
      data_0215fe84->childCount++;
    }
  }
}
