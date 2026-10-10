#include "types.h"

/* 1 when the map has an object of the given kind (MapObject.kind) that is not destroyed. */
int func_020da948(int kind);

BOOL Ov001_MapLacksObjectKind9(void) {
    return func_020da948(9) == 0;
}
