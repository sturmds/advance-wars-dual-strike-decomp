#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* needed to match: enum constants keep the cmp/moveq/movne order */
enum { SAME_TEAM, ENEMY };

/* 1 when an Oozium of another team than the current player's stands on the tile. */
int AiTile_HasEnemyOozium(int x, int y) {
  int slot = STATE->mapTiles->unitAt[TILE(x, y)];

  if (STATE->units[slot].type != UNIT_OOZIUM) {
    return 0;
  }
  return STATE->players[STATE->currentPlayer].team ==
                 STATE->players[(slot >> 6) + 1].team
             ? SAME_TEAM
             : ENEMY;
}
