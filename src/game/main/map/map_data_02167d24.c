#include "types.h"
#include "game/weather.h"

extern char data_02167cc0[];
extern char data_02167ccc[];
extern char data_02167cb8[];
extern char data_02167cbc[];

u8 data_02167d30[][2] = {
#include "data/main/data_02167d30.inc.c"
};
u8 gCoPicksDefault[][2] = {
#include "data/main/gCoPicksDefault.inc.c"
};
u32 data_02167d3c[] = {
#include "data/main/data_02167d3c.inc.c"
};
u32 data_02167d48[] = {
#include "data/main/data_02167d48.inc.c"
};
/* text id of each army's name */
u32 gArmyNameTextIds[] = {
#include "data/main/gArmyNameTextIds.inc.c"
};
u8 gDefaultCoList[] = {
#include "data/main/gDefaultCoList.inc.c"
};
u16 gOwnerCellCodes[] = {
#include "data/main/gOwnerCellCodes.inc.c"
};
WeatherFiles gMapPaletteFiles[] = {
#include "data/main/gMapPaletteFiles.inc.c"
};
