#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_hit_task.h"
#include "game/map_object.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* The kind of target at tile (x, y): MAP_HIT_PIPE_SEAM when the tile has seam hp left, else the kind of the
 * object units attack there, else 0. */
int Map_GetHitTargetKind(int x, int y) {
  MapObject* object;
  int seamHp;
  int kind;

  object = Map_GetObjectAt(x, y);
  kind = 0;
  seamHp = STATE->tileHp[x + STATE->mapTiles->rowStart[y]] & 0x7f;
  if (object != NULL) {
    kind = object->kind;
  }
  if (seamHp != 0) {
    kind = MAP_HIT_PIPE_SEAM;
  }
  return kind;
}
