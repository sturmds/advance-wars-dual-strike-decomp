#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Sets DISPCNT bit 8 in the current screen state. */
void Screen_ShowBg0(void) { data_027c0280->dispCnt |= 0x100; }
