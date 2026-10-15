#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"

void Font_SetDigitEndTile(int tile);
void func_020c4388(int a0, int a1, u16* a2);

void func_ov006_0235402c(void) {
  data_ov006_0236d640->unk_00 &= ~0x8000;
  Font_SetDigitEndTile(220);
  func_020c4388(16, 16, &MAP_STATE->cursorPixelX);
}
