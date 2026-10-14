#include "types.h"
#include "game/map_bg.h"

const u8 data_02143f38[] = {
#include "data/main/data_02143f38.inc.c"
};
const u8 data_02143f3c[] = {
#include "data/main/data_02143f3c.inc.c"
};
/* 4 BG tile entries per map cell, for layer 3 */
const u16 gMapCellGroundTiles[] = {
#include "data/main/gMapCellGroundTiles.inc.c"
};
/* same for layer 2: the part of a cell drawn into the row above */
const u16 gMapCellOverhangTiles[] = {
#include "data/main/gMapCellOverhangTiles.inc.c"
};
