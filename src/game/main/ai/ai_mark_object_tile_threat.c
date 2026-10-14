#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

typedef struct TilePos {
  s16 x;
  s16 y;
} TilePos;

/* Per object variant (bits 14-15 of the object's second halfword): its
 * tiles, ended by an x of 0xffff. */
extern TilePos* data_02167e98[3];

BOOL Ai_UnitUsesThreatMaps(int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sets threatSum to 255 on the tiles listed for the first active object of
 * kind 2 (unk_06 is 1), for unit types Ai_UnitUsesThreatMaps accepts. */
void Ai_MarkObjectTileThreat(void) {
  MapObject* object;
  TilePos* tile;

  if (!Ai_UnitUsesThreatMaps(STATE->actingUnit->type)) {
    return;
  }
  for (object = STATE->objects; object->kind != 0; object++) {
    if (object->kind == 2 && object->state == 1) {
      tile = data_02167e98[object->variant];
      for (;; tile++) {
        if ((u16)tile->x == 0xffff) {
          return;
        }
        STATE->mapTiles
            ->threatSum[tile->x + STATE->mapTiles->rowStart[tile->y]] = 255;
      }
    }
  }
}
