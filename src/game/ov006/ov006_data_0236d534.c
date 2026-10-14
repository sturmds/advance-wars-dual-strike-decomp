#include "types.h"

void LinkActionQueue_Push(void);

/* 1, 2, 3, 4 */
u8 Ov006_gSlotArmyColours[4] = {
#include "data/ov006/Ov006_gSlotArmyColours.inc.c"
};
u16 Ov006_gPortCells[4] = {
#include "data/ov006/Ov006_gPortCells.inc.c"
};
u16 Ov006_gComTowerCells[4] = {
#include "data/ov006/Ov006_gComTowerCells.inc.c"
};
u16 Ov006_gAirportCells[4] = {
#include "data/ov006/Ov006_gAirportCells.inc.c"
};
u16 Ov006_gHqCells[4] = {
#include "data/ov006/Ov006_gHqCells.inc.c"
};
u16 Ov006_gBaseCells[4] = {
#include "data/ov006/Ov006_gBaseCells.inc.c"
};
u16 Ov006_gCityCells[4] = {
#include "data/ov006/Ov006_gCityCells.inc.c"
};
u8 data_ov006_0236d588[2][16] = {
#include "data/ov006/data_ov006_0236d588.inc.c"
};
u8 data_ov006_0236d568[2][16] = {
#include "data/ov006/data_ov006_0236d568.inc.c"
};
u8 data_ov006_0236d5a8[2][16] = {
#include "data/ov006/data_ov006_0236d5a8.inc.c"
};
/* declared `u8 data_ov006_0236d5c8[2][16]` in ov006_find_army_layout.c: relocation at data_ov006_0236d5c8+0x14 is not on a 4-byte field */
u32 data_ov006_0236d5c8[8] = {
#include "data/ov006/data_ov006_0236d5c8.inc.c"
};
