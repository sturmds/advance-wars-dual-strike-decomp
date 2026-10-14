#include "types.h"
#include "game/script_slot.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02080440.h"
#include "game/map_bg.h"
#include "game/screen_contexts.h"
#include "game/task.h"

void MapScroll_BuildSpeedTable(void);
void MapView_DrawAllBgRows(void);
void MapView_DrawAllBgRowsGroundOnly(void);
void RangeSpread_Destroy(void);
void RangeSpread_Start(void);
void RangeSpread_Update(void);
extern u8 data_0217eb08[];
extern u8 data_0217fb08[];
void func_020f783c(void);
void func_020f931c(void);
void func_020f9540(void);
void func_020fa448(void);
void MapScroll_Step(void);
void MapView_DrawAllRangeTiles(void);
void MapView_DrawAllTiles(void);
void MapView_DrawScrolledEdges(void);
extern u8 data_02180b08[];
void func_020f8534(void);
void func_020f9558(void);
void Bg0AnimTask_Update(void);
void func_020f83d8(void);
void func_020f954c(void);
void func_020f9838(void);
void func_020fa6e8(void);
void MapView_ApplyBgScrollPanLayer0(void);
void Screen_SetDarkenBlend(void);
void func_020f8344(void);
void func_020fa6d8(void);
void func_020f85b0(void);
void func_020f8b90(void);
void func_020fa6c8(void);
void func_020f8394(void);
void func_020f8764(void);
void func_020f8b3c(void);
void func_020fa6b8(void);
void MapGfx_LoadGreyscale(void);
void MapView_ApplyBgScroll(void);
void Screen_SetLayer0Translucent(void);
void func_020f8b20(void);
void MapGfx_Load(void);
void func_020f84b0(void);
void func_020f85e0(void);
void func_020f8cb0(void);
void Screen_SetLayer0BlendForTilesetStrong(void);
void Weather_LoadGraphics(void);
void Screen_SetLayer0BlendForTileset(void);
void func_020f8ca8(void);
void Screen_SetLayer0Additive(void);
void Screen_SetLayer1Translucent(void);
void func_020f9564(void);
void func_020f8b74(void);
void Screen_ShowBg0(void);
void func_020f869c(void);
void MapGfx_LoadTilesetAndWeather(void);

char data_02169d78[] = {
#include "data/main/data_02169d78.inc.c"
};
u8 data_02169d8c[] = {
#include "data/main/data_02169d8c.inc.c"
};
u32 data_02169d7c[] = {
#include "data/main/data_02169d7c.inc.c"
};
u8 gCornerSpriteShape[] = {
#include "data/main/gCornerSpriteShape.inc.c"
};
u8 data_02169d94[] = {
#include "data/main/data_02169d94.inc.c"
};
u8 gCycleHookSteps[] = {
#include "data/main/gCycleHookSteps.inc.c"
};
TaskScriptCmd data_02169dac[] = {
#include "data/main/data_02169dac.inc.c"
};
u32 data_02169dbc[] = {
#include "data/main/data_02169dbc.inc.c"
};
ScriptCommand gMapScrollScript[] = {
#include "data/main/gMapScrollScript.inc.c"
};
/* mountain cells, picked by tile position */
u16 gMountainCellVariants[] = {
#include "data/main/gMountainCellVariants.inc.c"
};
u32 data_02169dd8[] = {
#include "data/main/data_02169dd8.inc.c"
};
TaskScriptCmd data_02169df8[] = {
#include "data/main/data_02169df8.inc.c"
};
u32 data_02169e18[] = {
#include "data/main/data_02169e18.inc.c"
};
/* tile map offset of each map tile column: two 32x32 screen blocks */
u16 data_02169eb8[] = {
#include "data/main/data_02169eb8.inc.c"
};
/* per terrain id; 0: keep the map's cell */
u16 data_02169e78[] = {
#include "data/main/data_02169e78.inc.c"
};
GfxLayer data_02169ef8[] = {
#include "data/main/data_02169ef8.inc.c"
};
Unk_02080440_50 data_02169f48[] = {
#include "data/main/data_02169f48.inc.c"
};
