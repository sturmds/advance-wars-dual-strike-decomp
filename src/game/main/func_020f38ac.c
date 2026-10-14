#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/info_panel.h"
#include "game/neighbor_dir.h"
#include "game/script_slot.h"

void AttackTargetSelect_Finish(void);
void CurrentPlayer_SetUnk33AndSendAction21(void);
void DailyFuel_BurnStep(void);
void FireTargets_Build(void);
void Game_EnterState18(void);
void Game_LeaveToModeTask(void);
void LinkPlayerDefeatTask_TransferProperties(void);
void LinkPlayerDefeatTask_Update(void);
void MapOverview_FadeInStep(void);
void MapOverview_WaitForClose(void);
void MenuDepth_Pop(void);
void MenuDepth_PopOnCurrentScreen(void);
void MenuDepth_PopOnOtherScreen(void);
void MenuDepth_Push(void);
void MenuDepth_PushOnCurrentScreen(void);
void MenuDepth_PushOnOtherScreen(void);
void RepairPick_Finish(void);
void RepairPick_Update(void);
void RepairTask_Init(void);
void SceneMode_IsChanging(void);
void ServiceCostPopup_Begin(void);
void ServiceCostPopup_CountDown(void);
void ServiceCostPopup_StartZoom(void);
void Supply_FlagAdjacentTargets(void);
void Supply_RefillAdjacentStep(void);
void TargetListPicker_Init(void);
void UnitDestroyTask_PlayDefaultEffectAndRemove(void);
void UnloadTask_Init(void);
void UnloadTask_PickTile(void);
void UnloadTask_Step(void);
void ZoomSpriteTask_Init(void);
void ZoomSpriteTask_InitWithSound(void);
void ZoomSpriteTask_Update(void);
extern u32 data_02166720[];
extern u8 data_02168328[];
extern u8 data_02168400[];
void func_02019bec(void);
void func_02021b34(void);
void func_020b9914(void);
void func_020c9778(void);
void func_020c98b4(void);
void func_020c9c70(void);
void func_020ca428(void);
void func_020ccd70(void);
void func_020cce88(void);
void func_020cdcb0(void);
void func_020ce17c(void);
void func_020cfe70(void);
void func_020d0148(void);
extern u8 gMapScrollScript[];
void MapOverview_Close(void);
void MapOverview_Open(void);
void MapOverview_RestoreCursor(void);
void ScreenWipe_PhaseDownInit(void);
void ScreenWipe_PhaseUpInit(void);
void TargetListPicker_Update(void);
void ZoomSpriteTask_UpdateThenResume(void);
extern u8 data_021669b0[];
void MapOverview_DimStep(void);
void ScreenWipe_PhaseDownStep(void);
void ScreenWipe_PhaseUpStep(void);
extern u8 data_02166c08[];

/* "002" */
char gTileCodeFileMap248First[4] = {
#include "data/main/gTileCodeFileMap248First.inc.c"
};
/* "003" */
char gTileCodeFileDefault[4] = {
#include "data/main/gTileCodeFileDefault.inc.c"
};
/* panel width by unit boxes shown */
u16 data_02166308[3] = {
#include "data/main/data_02166308.inc.c"
};
/* "/bmap" */
char gBmapDir[6] = {
#include "data/main/gBmapDir.inc.c"
};
u32 data_02166318[2] = {
#include "data/main/data_02166318.inc.c"
};
u32 data_02166338[2] = {
#include "data/main/data_02166338.inc.c"
};
u8 data_02166320[8] = {
#include "data/main/data_02166320.inc.c"
};
/* sprite shape of the picture */
u8 gInfoPanelUnitPictureShape[8] = {
#include "data/main/gInfoPanelUnitPictureShape.inc.c"
};
/* sprite shape of the terrain icon */
u8 data_02166348[8] = {
#include "data/main/data_02166348.inc.c"
};
/* sprite shape of the terrain picture */
u8 data_02166330[8] = {
#include "data/main/data_02166330.inc.c"
};
u8 data_02166310[8] = {
#include "data/main/data_02166310.inc.c"
};
InfoPanelRestPos gInfoPanelRestPos[INFO_PANEL_SIDES] = {
#include "data/main/gInfoPanelRestPos.inc.c"
};
s16 gInfoPanelWindowLeft[INFO_PANEL_SIDES][3] = {
#include "data/main/gInfoPanelWindowLeft.inc.c"
};
/* terrain box */
s16 data_0216635c[INFO_PANEL_SIDES][3] = {
#include "data/main/data_0216635c.inc.c"
};
u32 data_02166368[4] = {
#include "data/main/data_02166368.inc.c"
};
/* [owner - 1]: unit sprite shape with a badge */
void* data_021663b4[5] = {&data_02166368, &data_02166318, &data_02166368,
                          &data_02166318, &data_02166318};
void* data_021663c8[5] = {&data_02166338, &data_02166318, &data_02166338,
                          &data_02166318, &data_02166318};
NeighborDir data_02166378[4] = {
#include "data/main/data_02166378.inc.c"
};
/* declared `TileOffset* data_0216638c[]` in map_add_tile_offset.c: declared in src/game/main/map/map_add_tile_offset.c, with a type no header defines */
void* data_0216638c[5] = {0, &data_02166720, &data_02166720, &data_021669b0,
                          &data_02166c08};
u32 data_021663a0[5] = {
#include "data/main/data_021663a0.inc.c"
};
/* 3 per side of the unit: bit for data_022a6dd4, tile x offset, tile y offset */
s16 data_021663dc[12] = {
#include "data/main/data_021663dc.inc.c"
};
InfoPanelLayout data_021663f4 = {
#include "data/main/data_021663f4.inc.c"
};
TaskScriptCmd data_02166410[4] = {
#include "data/main/data_02166410.inc.c"
};
/* declared `u8 data_02166430[]` in info_panel_draw.c: relocation at data_02166430+0x4 is not on a 4-byte field */
u32 data_02166430[8] = {
#include "data/main/data_02166430.inc.c"
};
ScriptCommand data_02166450[4] = {
#include "data/main/data_02166450.inc.c"
};
/* declared `u8 data_02166470[]` in func_020cce70.c: relocation at data_02166470+0x0 is not on a 4-byte field */
u32 data_02166470[8] = {
#include "data/main/data_02166470.inc.c"
};
/* declared `const u8 data_02166490[]` in func_020cab68.c: relocation at data_02166490+0x0 is not on a 4-byte field */
u32 data_02166490[10] = {
#include "data/main/data_02166490.inc.c"
};
TaskScriptCmd data_021664b8[5] = {
#include "data/main/data_021664b8.inc.c"
};
TaskScriptCmd data_021664e0[5] = {
#include "data/main/data_021664e0.inc.c"
};
/* declared `const u8 data_02166508[]` in func_020cab50.c: relocation at data_02166508+0x0 is not on a 4-byte field */
u32 data_02166508[12] = {
#include "data/main/data_02166508.inc.c"
};
TaskScriptCmd data_02166538[6] = {
#include "data/main/data_02166538.inc.c"
};
TaskScriptCmd data_02166568[6] = {
#include "data/main/data_02166568.inc.c"
};
/* picture number of each unit type */
u16 data_02166598[28] = {
#include "data/main/data_02166598.inc.c"
};
/* declared `u8 data_021665d0[]` in func_020cfd30.c: relocation at data_021665d0+0x4 is not on a 4-byte field */
u32 data_021665d0[14] = {
#include "data/main/data_021665d0.inc.c"
};
TaskScriptCmd data_02166608[7] = {
#include "data/main/data_02166608.inc.c"
};
TaskScriptCmd gFireTargetTaskScript[7] = {
#include "data/main/gFireTargetTaskScript.inc.c"
};
TaskScriptCmd data_02166678[7] = {
#include "data/main/data_02166678.inc.c"
};
TaskScriptCmd data_021666b0[7] = {
#include "data/main/data_021666b0.inc.c"
};
ScriptCommand data_021666e8[7] = {
#include "data/main/data_021666e8.inc.c"
};
u32 data_02166720[14] = {
#include "data/main/data_02166720.inc.c"
};
/* per terrain, 2 bytes each: the icon number is the first byte */
u8 data_02166758[64] = {
#include "data/main/data_02166758.inc.c"
};
u32 data_02166798[16] = {
#include "data/main/data_02166798.inc.c"
};
u32 data_021667d8[20] = {
#include "data/main/data_021667d8.inc.c"
};
TaskScriptCmd data_02166888[12] = {
#include "data/main/data_02166888.inc.c"
};
TaskScriptCmd data_02166828[12] = {
#include "data/main/data_02166828.inc.c"
};
static TaskScriptCmd data_021668e8[12] = {
#include "data/main/data_021668e8.inc.c"
};
/* declared `EventScriptCommand gMapOverviewScript[]` in game_tick_player_timer.c: relocation at gMapOverviewScript+0x0 is not on a 4-byte field */
u32 gMapOverviewScript[24] = {
#include "data/main/gMapOverviewScript.inc.c"
};

void Scene_SetKind(int value);
int SceneMode_Request(int value);

/* Sets scene fields 0x00 to 0 and 0x0c to 5 (through Scene_SetKind and
 * SceneMode_Request). On the first screen it also starts the data_021668e8 child
 * task of parent. */
void func_020f38ac(Task* parent) {
  if (data_027c027c == data_027c0288.unk_00) {
    Scene_SetKind(0);
    SceneMode_Request(5);
    Task_CreateChild(data_021668e8, parent);
  } else {
    Scene_SetKind(0);
    SceneMode_Request(5);
  }
}
