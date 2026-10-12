#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Writes the 2x2 tiles of a skill's icon at tile (x, y) of layer 0 (a 32 tile
 * wide map, palette 2). The icons start at tile 0x200, four tiles each, for
 * skill id 32 and up. The inline expressions are needed to match. */
void ForceRankScreen_PutSkillIcon(int x, int y, int skill) {
  data_027c0280->layer0Map[x + y * 32] = ((skill - 32) * 4 + 0x200) | 0x2000;
  data_027c0280->layer0Map[x + y * 32 + 1] =
      ((skill - 32) * 4 + 0x201) | 0x2000;
  data_027c0280->layer0Map[x + y * 32 + 32] =
      ((skill - 32) * 4 + 0x202) | 0x2000;
  data_027c0280->layer0Map[x + y * 32 + 33] =
      ((skill - 32) * 4 + 0x203) | 0x2000;
}
