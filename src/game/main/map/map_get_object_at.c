#include "types.h"
#include "game/map_object.h"

MapObject *func_020db31c(int x, int y);           /* object whose area covers (x, y); NULL: none */
BOOL func_020dafc4(MapObject *object, s16 *pos);  /* the tile units attack the object on; FALSE: kind has none */

/* Returns the object that units attack at (x, y), or NULL when the tile is
 * not an object's target tile or the object is destroyed. */
MapObject *Map_GetObjectAt(int x, int y)
{
    s16 pos[2];
    MapObject *object = func_020db31c(x, y);

    if (object == NULL) {
        return NULL;
    }
    if (!func_020dafc4(object, pos)) {
        return NULL;
    }
    if (pos[0] != x || pos[1] != y) {
        return NULL;
    }
    if (object->hp == 0) {
        object = NULL;
    }
    return object;
}
