#include "types.h"
#include "game/Unk_0215fe84.h"

/* Counts the connected children (aids 1 to 7). */
void Wireless_CountChildren(void) {
  int aid;

  data_0215fe84->childCount = 0;
  for (aid = 1; aid < 8; aid++) {
    if (data_0215fe84->childAids & (1 << aid)) {
      data_0215fe84->childCount++;
    }
  }
}
