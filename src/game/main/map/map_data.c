#include "types.h"
#include "game/task.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void func_020d3bf8(void);

/* "0b3" */
char gWeatherCloudFileName[4] = {
#include "data/main/gWeatherCloudFileName.inc.c"
};
char gTilesetPaletteFileDefault[4] = {
#include "data/main/gTilesetPaletteFileDefault.inc.c"
};
char gTilesetPaletteFileAlt[4] = {
#include "data/main/gTilesetPaletteFileAlt.inc.c"
};
TaskScriptCmd data_02166d98[6] = {
#include "data/main/data_02166d98.inc.c"
};
u8 data_02166dc8[64] = {
#include "data/main/data_02166dc8.inc.c"
};
