#include "types.h"

extern u16* data_0215fe84;
void Wireless_CountChildren(int, void*);

void Wireless_SetChildAids(int a0) {
  u16* t0 = data_0215fe84;
  *t0 = a0;
  Wireless_CountChildren(a0, t0);
}
