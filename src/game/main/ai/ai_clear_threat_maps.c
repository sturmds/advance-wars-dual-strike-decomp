#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sets MapTiles.threatMax and threatSum to 0 for every tile. */
void Ai_ClearThreatMaps(void) {
  TileMap_Fill(STATE->mapTiles->threatMax, 0);
  TileMap_Fill(STATE->mapTiles->threatSum, 0);
}
