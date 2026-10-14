#include "types.h"

void LinkActionQueue_Push(void);

/* 1, 2, 3, 4 */
u8 Ov006_gSlotArmyColours[] = {
#include "data/ov006/Ov006_gSlotArmyColours.inc.c"
};
u16 Ov006_gPortCells[] = {
#include "data/ov006/Ov006_gPortCells.inc.c"
};
u16 Ov006_gComTowerCells[] = {
#include "data/ov006/Ov006_gComTowerCells.inc.c"
};
u16 Ov006_gAirportCells[] = {
#include "data/ov006/Ov006_gAirportCells.inc.c"
};
u16 Ov006_gHqCells[] = {
#include "data/ov006/Ov006_gHqCells.inc.c"
};
u16 Ov006_gBaseCells[] = {
#include "data/ov006/Ov006_gBaseCells.inc.c"
};
u16 Ov006_gCityCells[] = {
#include "data/ov006/Ov006_gCityCells.inc.c"
};
u8 data_ov006_0236d588[][16] = {
#include "data/ov006/data_ov006_0236d588.inc.c"
};
u8 data_ov006_0236d568[][16] = {
#include "data/ov006/data_ov006_0236d568.inc.c"
};
u8 data_ov006_0236d5a8[][16] = {
#include "data/ov006/data_ov006_0236d5a8.inc.c"
};
/* declared `u8 data_ov006_0236d5c8[2][16]` in ov006_find_army_layout.c: relocation at data_ov006_0236d5c8+0x14 is not on a 4-byte field */
u32 data_ov006_0236d5c8[] = {
#include "data/ov006/data_ov006_0236d5c8.inc.c"
};
