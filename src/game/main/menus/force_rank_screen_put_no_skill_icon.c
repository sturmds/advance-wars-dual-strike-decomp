#include "types.h"
#include "game/Unk_02088d04.h"

/* Writes the 2x2 tiles of the "no skill" icon at tile (x, y) of layer 0
 * (a 32 tile wide map, palette 2). */
void ForceRankScreen_PutNoSkillIcon(int x, int y) {
  int tile = x + y * 32;

  data_027c0280->layer0Map[tile] = 0x21fc;
  data_027c0280->layer0Map[tile + 1] = 0x21fd;
  data_027c0280->layer0Map[tile + 32] = 0x21fe;
  data_027c0280->layer0Map[tile + 33] = 0x21ff;
}
