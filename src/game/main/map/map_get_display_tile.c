#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

MapObject* Map_FindObjectCovering(int x, int y);
void MapObject_GetKindOffset(int kind, s16* offset);

/* Tile byte (terrain and owner) the info panel and cursor show for a tile.
 * A tile covered by a map object shows the tile at the object's origin,
 * moved by the offset MapObject_GetKindOffset gives for its kind. */
u8 Map_GetDisplayTile(int x, int y) {
  int tile;
  MapObject* object;
  s16 offset[2];

  tile = MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]];
  object = Map_FindObjectCovering(x, y);
  if (object != NULL) {
    MapObject_GetKindOffset(object->kind, offset);
    tile = MAP_STATE->mapTiles
               ->tiles[object->x + offset[0] +
                       MAP_STATE->mapTiles->rowStart[object->y + offset[1]]];
  }
  return tile;
}
