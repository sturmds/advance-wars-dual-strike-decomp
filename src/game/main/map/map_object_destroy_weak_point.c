#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"
#include "game/task.h"
#include "game/terrain.h"

void func_020eed28(int x, int y, Task* parent);
void func_020c4018(void);
void MapObject_Remove(MapObject* object);
void func_020c4448(void);

/* Map cell code that replaces a destroyed Grand Bolt weak point; its
 * terrain becomes TERRAIN_GRAND_BOLT. */
#define CELL_WEAK_POINT_DESTROYED 0x17a

/* A destroyed Grand Bolt weak point (object kinds 11 to 13): starts the
 * destruction effect (func_020eed28), changes its tile, removes the object
 * from the object list (MapObject_Remove moves the later entries down) and
 * rescans the properties. */
void MapObject_DestroyWeakPoint(MapObject* object, Task* task) {
  func_020eed28(object->x, object->y, task);
  MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(object->x, object->y)] =
      TERRAIN_GRAND_BOLT;
  MAP_STATE->mapTiles->mapCells[MAP_TILE_INDEX(object->x, object->y)] =
      CELL_WEAK_POINT_DESTROYED;
  func_020c4018();
  MapObject_Remove(object);
  func_020c4448();
}
