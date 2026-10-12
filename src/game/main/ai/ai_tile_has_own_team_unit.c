#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* 1 when a unit stands on the tile and belongs to the current player's team. */
int AiTile_HasOwnTeamUnit(int x, int y) {
  int slot = STATE->mapTiles->unitAt[TILE(x, y)];

  if (slot == 0) {
    return 0;
  }
  return STATE->players[STATE->currentPlayer].team ==
         STATE->players[(slot >> 6) + 1].team;
}
