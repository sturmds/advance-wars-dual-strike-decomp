#include "types.h"
#include "game/Unk_02080440.h"

int Ov006_IsRiverOrBridgeOnRiver(int x, int y);

int func_ov006_02360308(int x, int y) {
  int mask = 0;

  if (y > 0) {
    mask |= Ov006_IsRiverOrBridgeOnRiver(x, y - 1) << 3;
  }
  if (x > 0) {
    mask |= Ov006_IsRiverOrBridgeOnRiver(x - 1, y) << 2;
  }
  if (x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth - 1) {
    mask |= Ov006_IsRiverOrBridgeOnRiver(x + 1, y) << 1;
  }
  if (y < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapHeight - 1) {
    mask |= Ov006_IsRiverOrBridgeOnRiver(x, y + 1);
  }
  return mask;
}
