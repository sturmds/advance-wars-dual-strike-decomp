#include "types.h"
#include "game/map_object.h"

/* needed to match: int mark where the definitions take s8, and u16 x and y
 * for the cone */
void RangeMap_FillRowAndColumn(int x, int y, int value);
void RangeMap_FillColumnsBelow(int x, u16 y, int value);
void RangeMap_FillCone(u16 x, u16 y, int dir, int length, int value);

/* Writes mark into the range map on every tile the object's shot covers: the
 * Laser's row and column, a cannon's cone in its facing, or the columns below
 * a Deathray. Returns FALSE for a destroyed object or a kind with no shot.
 * Called by MapCursor_ShowRange (the B-button range display, mark 0) and
 * Ai_AddObjectThreat (the AI's threat map, mark -1). */
BOOL MapObject_MarkFireRange(MapObject* object, int mark) {
  int variant;
  int kind;
  int dx;

  if (object->hp == 0) {
    return FALSE;
  }
  kind = object->kind;
  switch (kind) {
    case MAP_OBJECT_LASER:
      RangeMap_FillRowAndColumn(object->x, object->y, mark);
      return TRUE;
    case MAP_OBJECT_BLACK_CANNON:
      variant = object->variant;
      dx = MapObject_GetShotOriginX(kind, variant);
      RangeMap_FillCone(object->x + dx,
                        object->y + MapObject_GetShotOriginY(kind, variant),
                        variant, BLACK_CANNON_RANGE, mark);
      return TRUE;
    case MAP_OBJECT_MINICANNON:
      variant = object->variant;
      RangeMap_FillCone(object->x + gMinicannonShotOrigins[variant * 2],
                        object->y + gMinicannonShotOrigins[variant * 2 + 1],
                        variant, MINICANNON_RANGE, mark);
      return TRUE;
    case MAP_OBJECT_DEATHRAY:
      /* from the row below the object */
      RangeMap_FillColumnsBelow(object->x, object->y + DEATHRAY_SIZE, mark);
      return TRUE;
    default:
      return FALSE;
  }
}
