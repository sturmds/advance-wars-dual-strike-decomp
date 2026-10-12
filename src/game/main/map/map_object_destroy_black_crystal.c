#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"
#include "game/task.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void TileEffectTask_Create(int x, int y, int effect, Task* parent);
void func_020c4018(void);
void MapObject_Remove(MapObject* object);
void func_020c4448(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* A destroyed Black Crystal: plays effect 29 on its tile, turns the tile
 * into plain ground, removes the object from the object list
 * (MapObject_Remove moves the later entries down) and rescans the properties. */
void MapObject_DestroyBlackCrystal(MapObject* object, Task* task) {
  TileEffectTask_Create(object->x, object->y, 29, task);
  STATE->mapTiles->tiles[TILE_INDEX(object->x, object->y)] = TERRAIN_PLAIN;
  STATE->mapTiles->mapCells[TILE_INDEX(object->x, object->y)] = 4;
  func_020c4018();
  MapObject_Remove(object);
  func_020c4448();
}
