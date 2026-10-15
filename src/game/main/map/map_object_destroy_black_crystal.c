#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"
#include "game/task.h"
#include "game/terrain.h"

void TileEffectTask_Create(int x, int y, int effect, Task* parent);
void func_020c4018(void);
void MapObject_Remove(MapObject* object);
void func_020c4448(void);

/* A destroyed Black Crystal: plays effect 29 on its tile, turns the tile
 * into plain ground, removes the object from the object list
 * (MapObject_Remove moves the later entries down) and rescans the properties. */
void MapObject_DestroyBlackCrystal(MapObject* object, Task* task) {
  TileEffectTask_Create(object->x, object->y, 29, task);
  MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(object->x, object->y)] =
      TERRAIN_PLAIN;
  MAP_STATE->mapTiles->mapCells[MAP_TILE_INDEX(object->x, object->y)] = 4;
  func_020c4018();
  MapObject_Remove(object);
  func_020c4448();
}
