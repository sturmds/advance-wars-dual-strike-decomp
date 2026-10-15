#include "types.h"
#include "game/Unk_02088d04.h"

/* Makes state the screen that display code draws to: its registers,
 * BG layers and VRAM pointers are read through data_027c0280. */
void Screen_SetCurrentGfx(Unk_027c0280* state) { data_027c0280 = state; }
