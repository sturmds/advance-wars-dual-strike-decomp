#include "types.h"
#include "game/script_slot.h"
#include "game/map_object.h"
#include "game/song.h"
#include "game/task.h"
#include "game/tile_point.h"

void CannonShotTask_PlayImpact(void);
void CannonShotTask_PlayMuzzleFlash(void);
void CannonShotTask_ViewTarget(void);
void EventScript_IsRunning(void);
void MapObjectFireTask_FinishWhenDone(void);
void MapObjectFireTask_HitNextUnit(void);
void MapObjectTurnTask_Init(void);
void MapObjectTurnTask_TargetNext(void);
void MapObjects_InitState(void);
void MapObjects_TickCountdowns(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void ObjectHealScan_Reset(void);
void ObjectHealScan_StartNext(void);
void ObjectHealTask_LoadGraphics(void);
void ObjectHealTask_PlayEffect(void);
void ObjectHealTask_RepairUnits(void);
void Screen_SetBlendTopOnly(void);
void TileListTask_MoveToNext(void);
void TileListTask_PlayEffect(void);
void TileStrikeTask_Init2d(void);
void TileStrikeTask_Init3d(void);
void TileStrikeTask_UpdateFall2d(void);
void TileStrikeTask_UpdateFall3d(void);
void TileStrikeTask_UpdateImpact2d(void);
void TileStrikeTask_UpdateImpact3d(void);
extern const u8 data_02148000[];
extern char data_02167cd4[];
extern char data_02167ce8[];
extern char data_02167cf0[];
extern TilePointU16 data_02167f50[];
extern TaskScriptCmd data_021680e0[];
void func_02021b34(void);
void func_020d8c8c(void);
void func_020d8e70(void);
void func_020d8f14(void);
void func_020d8fac(void);
void func_020d9234(void);
void func_020d9ea8(void);
extern ScriptCommand gMapScrollScript[];
void MapObjectFireTask_ShowNextShot(void);
extern TilePointU16 data_02167f84[];

void* data_02167e88 = &data_02167cd4;
/* 0x10 bytes no code names, before data_02167e94, cut in 4 to keep the sizes ascending */
void* data_02167e84 = &data_02167cf0;
void* data_02167e90 = &data_02167ce8;
TilePointU16 data_02167e94[] = {
#include "data/main/data_02167e94.inc.c"
};
const void* data_02167e8c = data_02148000;
/* Per object variant: its tiles (see map_object.h) */
TilePointU16* data_02167e98[] = {data_02167e94, data_02167f50, data_02167f84};
int gBlackCannonShotOrigins[] = {
#include "data/main/gBlackCannonShotOrigins.inc.c"
};
int gMapObjectConeLengths[] = {
#include "data/main/gMapObjectConeLengths.inc.c"
};
int gMinicannonShotOrigins[] = {
#include "data/main/gMinicannonShotOrigins.inc.c"
};
TaskScriptCmd data_02167ef0[] = {
#include "data/main/data_02167ef0.inc.c"
};
TaskScriptCmd data_02167f20[] = {
#include "data/main/data_02167f20.inc.c"
};
TilePointU16 data_02167f84[] = {
#include "data/main/data_02167f84.inc.c"
};
TilePointU16 data_02167f50[] = {
#include "data/main/data_02167f50.inc.c"
};
TaskScriptCmd data_02167fb8[] = {
#include "data/main/data_02167fb8.inc.c"
};
TaskScriptCmd data_02167ff0[] = {
#include "data/main/data_02167ff0.inc.c"
};
TaskScriptCmd data_02168088[] = {
#include "data/main/data_02168088.inc.c"
};
TaskScriptCmd data_02168030[] = {
#include "data/main/data_02168030.inc.c"
};
TaskScriptCmd data_021680e0[] = {
#include "data/main/data_021680e0.inc.c"
};
TaskScriptCmd data_02168140[] = {
#include "data/main/data_02168140.inc.c"
};
