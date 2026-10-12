#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern u8 data_ov006_0236b62c[];

/* When at least 7 of the 8 slots have bit 0 set (flags 0x10 clear, toolResult is 6),
 * puts one sprite at x 0x40 or 0xc0 depending on unitMode. The 8th slot is
 * unk_d4, so the loop reads one slot past the unk_80 array. */
void func_ov006_023547bc(void) {
  int i;
  int count = 0;
  int x;
  Unk_ov006_0235f690* state = data_ov006_0236d640;

  for (i = 32; i <= 39; i++) {
    if (state->unk_80[i - 32].flags & 1) {
      count++;
    }
  }
  if (count < 7) {
    return;
  }
  if (state->unk_00 & 0x10) {
    return;
  }
  if (state->toolResult != 6) {
    return;
  }
  if (state->unitMode == 1) {
    x = 0x40;
  } else {
    x = 0xc0;
  }
  Oam_PutSprite(x & 0x1ff, 0, data_ov006_0236b62c, 0);
}
