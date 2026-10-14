#include "types.h"
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

u32 data_02166ec4[2] = {
#include "data/main/data_02166ec4.inc.c"
};
u32 data_02166ebc[2] = {
#include "data/main/data_02166ebc.inc.c"
};
u32 data_02166ecc[2] = {
#include "data/main/data_02166ecc.inc.c"
};
u8 data_02166eb4[8] = {
#include "data/main/data_02166eb4.inc.c"
};
/* declared `u8 data_02166ed4[]` in func_020d5b0c.c: relocation at data_02166ed4+0xc is not on a 4-byte field */
u32 data_02166ed4[10] = {
#include "data/main/data_02166ed4.inc.c"
};
/* declared `u8 data_02166efc[]` in func_020d5b0c.c: relocation at data_02166efc+0xc is not on a 4-byte field */
u32 data_02166efc[10] = {
#include "data/main/data_02166efc.inc.c"
};
TaskScriptCmd data_02166f24[7] = {
#include "data/main/data_02166f24.inc.c"
};
TaskScriptCmd data_02166f5c[8] = {
#include "data/main/data_02166f5c.inc.c"
};
/* declared `MapPowerLine* data_02166f9c[]` in co_quote_start_map_line.c: declared in src/game/main/script/co_quote_start_map_line.c, with a type no header defines */
void* data_02166f9c[42] = {0,
                           0,
                           0,
                           &data_02166ebc,
                           &data_02166ecc,
                           &data_02166ec4,
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
