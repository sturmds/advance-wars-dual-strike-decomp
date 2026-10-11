#include "types.h"
#include "game/map_object.h"

extern int data_02167ed0[];

void RangeMap_FillRowAndColumn(int x, int y, int value);
void RangeMap_FillColumnsBelow(int x, u16 y, int value);
void func_01ffad44(u16 x, u16 y, int variant, int size, int value);
int MapObject_GetDrawOffsetY(int kind, int variant);
int MapObject_GetDrawOffsetX(int kind, int variant);

/* Draws a map object that is still standing; FALSE for a destroyed one or a
 * kind with no drawing here. Kinds 1, 3, 4 and 5 are handled. */
BOOL MapObject_Draw(MapObject *object, int value)
{
    int variant;
    int kind;
    int dx;

    if (object->hp == 0) {
        return FALSE;
    }
    kind = object->kind;
    switch (kind) {
    case 1:
        RangeMap_FillRowAndColumn(object->x, object->y, value);
        return TRUE;
    case 3:
        variant = object->variant;
        dx = MapObject_GetDrawOffsetX(kind, variant);
        func_01ffad44(object->x + dx, object->y + MapObject_GetDrawOffsetY(kind, variant), variant, 10, value);
        return TRUE;
    case 4:
        variant = object->variant;
        func_01ffad44(object->x + data_02167ed0[variant * 2], object->y + data_02167ed0[variant * 2 + 1], variant, 4, value);
        return TRUE;
    case 5:
        RangeMap_FillColumnsBelow(object->x, object->y + 3, value);
        return TRUE;
    default:
        return FALSE;
    }
}
