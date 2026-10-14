#include "types.h"
#include "game/weather.h"

extern u8 data_02167cc0[];
extern u8 data_02167ccc[];
extern u8 data_02167cb8[];
extern u8 data_02167cbc[];

u8 data_02167d30[5][2] = {
#include "data/main/data_02167d30.inc.c"
};
u8 gCoPicksDefault[5][2] = {
#include "data/main/gCoPicksDefault.inc.c"
};
u32 data_02167d3c[3] = {
#include "data/main/data_02167d3c.inc.c"
};
u32 data_02167d48[4] = {
#include "data/main/data_02167d48.inc.c"
};
/* text id of each army's name */
u32 gArmyNameTextIds[6] = {
#include "data/main/gArmyNameTextIds.inc.c"
};
u8 gDefaultCoList[28] = {
#include "data/main/gDefaultCoList.inc.c"
};
u16 gOwnerCellCodes[36] = {
#include "data/main/gOwnerCellCodes.inc.c"
};
WeatherFiles gMapPaletteFiles[4] = {
#include "data/main/gMapPaletteFiles.inc.c"
};
