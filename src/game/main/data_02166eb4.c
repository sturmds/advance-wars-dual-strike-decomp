#include "types.h"
#include "game/map_power_line.h"
#include "game/task.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void PowerActivateTask_LoadWindowGfx(void);
void PowerActivateTask_Run(void);
void PowerQuoteTask_Run(void);
void func_020d5324(void);
void func_020d55f4(void);
void func_020d5620(void);
void func_020d5ae8(void);
void TileSpriteTask_Animate(void);
void TileSpriteTask_CreateSprite(void);
void TileSpriteTask_DrawObject(void);

MapPowerLine data_02166ec4[] = {
#include "data/main/data_02166ec4.inc.c"
};
MapPowerLine data_02166ebc[] = {
#include "data/main/data_02166ebc.inc.c"
};
MapPowerLine data_02166ecc[] = {
#include "data/main/data_02166ecc.inc.c"
};
u8 data_02166eb4[] = {
#include "data/main/data_02166eb4.inc.c"
};
TaskScriptCmd data_02166ed4[] = {
#include "data/main/data_02166ed4.inc.c"
};
TaskScriptCmd data_02166efc[] = {
#include "data/main/data_02166efc.inc.c"
};
TaskScriptCmd data_02166f24[] = {
#include "data/main/data_02166f24.inc.c"
};
TaskScriptCmd data_02166f5c[] = {
#include "data/main/data_02166f5c.inc.c"
};
MapPowerLine* data_02166f9c[] = {0,
                                 0,
                                 0,
                                 data_02166ebc,
                                 data_02166ecc,
                                 data_02166ec4,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0};
