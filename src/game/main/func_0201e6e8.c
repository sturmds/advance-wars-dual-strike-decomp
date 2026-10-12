#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;
void func_0201e68c(int x, int y, int width, int height, u16* map, int palette);

void func_0201e6e8(int x, int y, int width, int height) {
  func_0201e68c(x, y, width, height, data_027c0280->layer1Map, 0);
}
