#include "types.h"

BOOL Battle_IsSideAir(int side);
void func_ov002_0235e920(void *tilemap, u8 side, u8 isAir);
void func_ov002_0235e874(void *tilemap, u8 side, u8 isAir);
void func_ov002_0235e720(void *tilemap, u8 side, u8 isAir);
void Battle_PutSideBlock750(void *tilemap, u8 side, u8 isAir);

/* Writes the tile maps of both sides into tilemap in four steps (callees not
 * named yet, at least one calls Bg_PutTileBlock). isAir is passed to each. */
void Battle_BuildSideTilemaps(void *tilemap)
{
    int side;
    BOOL isAir;

    for (side = 0; side < 2; side++) {
        isAir = Battle_IsSideAir(side) ? TRUE : FALSE;
        func_ov002_0235e920(tilemap, side, isAir);
        func_ov002_0235e874(tilemap, side, isAir);
        func_ov002_0235e720(tilemap, side, isAir);
        Battle_PutSideBlock750(tilemap, side, isAir);
    }
}
