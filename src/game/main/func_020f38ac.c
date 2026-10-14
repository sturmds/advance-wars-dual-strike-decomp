#include "types.h"
#include "game/terrain.h"
#include "game/event_script.h"
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
extern TaskScriptCmd data_02168328[];
extern TaskScriptCmd data_02168400[];
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
extern ScriptCommand gMapScrollScript[];
void MapOverview_Close(void);
void MapOverview_Open(void);
void MapOverview_RestoreCursor(void);
void ScreenWipe_PhaseDownInit(void);
void ScreenWipe_PhaseUpInit(void);
void TargetListPicker_Update(void);
void ZoomSpriteTask_UpdateThenResume(void);
extern u32 data_021669b0[];
void MapOverview_DimStep(void);
void ScreenWipe_PhaseDownStep(void);
void ScreenWipe_PhaseUpStep(void);
extern u32 data_02166c08[];

/* "002" */
char gTileCodeFileMap248First[] = {
#include "data/main/gTileCodeFileMap248First.inc.c"
};
/* "003" */
char gTileCodeFileDefault[] = {
#include "data/main/gTileCodeFileDefault.inc.c"
};
/* panel width by unit boxes shown */
u16 data_02166308[INFO_PANEL_WIDTH_COUNT] = {
#include "data/main/data_02166308.inc.c"
};
/* "/bmap" */
char gBmapDir[] = {
#include "data/main/gBmapDir.inc.c"
};
u32 data_02166318[] = {
#include "data/main/data_02166318.inc.c"
};
u32 data_02166338[] = {
#include "data/main/data_02166338.inc.c"
};
u8 data_02166320[] = {
#include "data/main/data_02166320.inc.c"
};
/* sprite shape of the picture */
u8 gInfoPanelUnitPictureShape[] = {
#include "data/main/gInfoPanelUnitPictureShape.inc.c"
};
/* sprite shape of the terrain icon */
u8 data_02166348[] = {
#include "data/main/data_02166348.inc.c"
};
/* sprite shape of the terrain picture */
u8 data_02166330[] = {
#include "data/main/data_02166330.inc.c"
};
u8 data_02166310[] = {
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
u32 data_02166368[] = {
#include "data/main/data_02166368.inc.c"
};
/* [owner - 1]: unit sprite shape with a badge */
void* data_021663b4[] = {&data_02166368, &data_02166318, &data_02166368,
                         &data_02166318, &data_02166318};
void* data_021663c8[] = {&data_02166338, &data_02166318, &data_02166338,
                         &data_02166318, &data_02166318};
NeighborDir data_02166378[] = {
#include "data/main/data_02166378.inc.c"
};
/* declared `TileOffset* data_0216638c[]` in map_add_tile_offset.c: declared in src/game/main/map/map_add_tile_offset.c, with a type no header defines */
void* data_0216638c[] = {0, &data_02166720, &data_02166720, &data_021669b0,
                         &data_02166c08};
u32 data_021663a0[] = {
#include "data/main/data_021663a0.inc.c"
};
/* 3 per side of the unit: bit for data_022a6dd4, tile x offset, tile y offset */
s16 data_021663dc[] = {
#include "data/main/data_021663dc.inc.c"
};
InfoPanelLayout data_021663f4 = {
#include "data/main/data_021663f4.inc.c"
};
TaskScriptCmd data_02166410[] = {
#include "data/main/data_02166410.inc.c"
};
TaskScriptCmd data_02166430[] = {
#include "data/main/data_02166430.inc.c"
};
ScriptCommand data_02166450[] = {
#include "data/main/data_02166450.inc.c"
};
ScriptCommand data_02166470[] = {
#include "data/main/data_02166470.inc.c"
};
ScriptCommand data_02166490[] = {
#include "data/main/data_02166490.inc.c"
};
TaskScriptCmd data_021664b8[] = {
#include "data/main/data_021664b8.inc.c"
};
TaskScriptCmd data_021664e0[] = {
#include "data/main/data_021664e0.inc.c"
};
ScriptCommand data_02166508[] = {
#include "data/main/data_02166508.inc.c"
};
TaskScriptCmd data_02166538[] = {
#include "data/main/data_02166538.inc.c"
};
TaskScriptCmd data_02166568[] = {
#include "data/main/data_02166568.inc.c"
};
/* picture number of each unit type */
u16 data_02166598[] = {
#include "data/main/data_02166598.inc.c"
};
TaskScriptCmd data_021665d0[] = {
#include "data/main/data_021665d0.inc.c"
};
TaskScriptCmd data_02166608[] = {
#include "data/main/data_02166608.inc.c"
};
TaskScriptCmd gFireTargetTaskScript[] = {
#include "data/main/gFireTargetTaskScript.inc.c"
};
TaskScriptCmd data_02166678[] = {
#include "data/main/data_02166678.inc.c"
};
TaskScriptCmd data_021666b0[] = {
#include "data/main/data_021666b0.inc.c"
};
ScriptCommand data_021666e8[] = {
#include "data/main/data_021666e8.inc.c"
};
u32 data_02166720[] = {
#include "data/main/data_02166720.inc.c"
};
TerrainIcon data_02166758[] = {
#include "data/main/data_02166758.inc.c"
};
EventScriptCommand data_02166798[] = {
#include "data/main/data_02166798.inc.c"
};
EventScriptCommand data_021667d8[] = {
#include "data/main/data_021667d8.inc.c"
};
TaskScriptCmd data_02166888[] = {
#include "data/main/data_02166888.inc.c"
};
TaskScriptCmd data_02166828[] = {
#include "data/main/data_02166828.inc.c"
};
static TaskScriptCmd data_021668e8[] = {
#include "data/main/data_021668e8.inc.c"
};
ScriptCommand gMapOverviewScript[] = {
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
