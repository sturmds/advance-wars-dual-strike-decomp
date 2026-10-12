#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
int Ov006_IsBareShoal(int x, int y);

int func_ov006_02363cb8(int x, int y) {
  int mask = 0;

  if (y > 0) {
    mask |= Ov006_IsBareShoal(x, y - 1) << 3;
  }
  if (x > 0) {
    mask |= Ov006_IsBareShoal(x - 1, y) << 2;
  }
  if (x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth - 1) {
    mask |= Ov006_IsBareShoal(x + 1, y) << 1;
  }
  if (y < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapHeight - 1) {
    mask |= Ov006_IsBareShoal(x, y + 1);
  }
  return mask;
}
