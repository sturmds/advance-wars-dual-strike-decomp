#include "types.h"
#include "game/Unk_0215fe84.h"

void Wireless_CountChildren(int, void*);

void Wireless_SetChildAids(int a0) {
  Unk_0215fe84* t0 = data_0215fe84;
  t0->childAids = a0;
  Wireless_CountChildren(a0, t0);
}
