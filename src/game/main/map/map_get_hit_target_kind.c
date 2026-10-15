#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_hit_task.h"
#include "game/map_object.h"

/* The kind of target at tile (x, y): MAP_HIT_PIPE_SEAM when the tile has seam hp left, else the kind of the
 * object units attack there, else 0. */
int Map_GetHitTargetKind(int x, int y) {
  MapObject* object;
  int seamHp;
  int kind;

  object = Map_GetObjectAt(x, y);
  kind = 0;
  seamHp = MAP_STATE->tileHp[x + MAP_STATE->mapTiles->rowStart[y]] & 0x7f;
  if (object != NULL) {
    kind = object->kind;
  }
  if (seamHp != 0) {
    kind = MAP_HIT_PIPE_SEAM;
  }
  return kind;
}
