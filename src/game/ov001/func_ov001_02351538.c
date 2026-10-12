#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_020f5878(int x, int y);

void func_ov001_02351538(void) {
  int x;
  int y;

  for (y = 0; y < GAME->unk_1c4->mapHeight; y++) {
    for (x = 0; x < GAME->unk_1c4->mapWidth; x++) {
      if (GAME->mapTiles->unk_2338[GAME->mapTiles->rowStart[y] + x] != 0) {
        MapUnit* unit =
            &GAME->units[GAME->mapTiles
                             ->unitAt[x + GAME->mapTiles->rowStart[y]]];

        if (unit->type == UNIT_OOZIUM) {
          func_020f5878(unit->x, unit->y);
          return;
        }
      }
    }
  }
}
