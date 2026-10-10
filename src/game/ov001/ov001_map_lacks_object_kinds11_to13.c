#include "types.h"

/* 1 when the map has an object of the given kind (MapObject.kind) that is not destroyed. */
int func_020da948(int kind);

BOOL Ov001_MapLacksObjectKinds11To13(void) {
    if (func_020da948(11)) {
        return 0;
    }
    if (func_020da948(12)) {
        return 0;
    }
    return func_020da948(13) == 0;
}
