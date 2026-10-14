#include "types.h"
#include "game/event_script.h"
#include "game/song.h"
#include "game/task.h"

void BlackBombBlast_Hit(void);
void BlackBombBlast_InitPosition(void);
void BlackBombExplosion_StartBlast(void);
void BlackBomb_ConfirmExplodeStep(void);
void CaptureCountDown_Update(void);
void CaptureCountUp_Update(void);
void CaptureDone_Draw(void);
void CaptureDone_Init(void);
void CapturePicture_Draw(void);
void CapturePicture_FreeSprites(void);
void CapturePicture_Init(void);
void CaptureShake_Start(void);
void CaptureShake_Update(void);
void CaptureTask_ShowCompletion(void);
void Game_EnterState18(void);
void MapHitTask_ShowResult(void);
void MapSpriteTask_LoadKindGraphics(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void MissileStrike_Explode(void);
void MissileStrike_FallStep(void);
void MissileStrike_Init(void);
void MissileStrike_ScrollToTarget(void);
void MissileStrike_StartBlastAnim(void);
void SiloMissileFall_Init(void);
void SiloMissileFall_Update(void);
void SiloMissileLaunch_Init(void);
void SiloMissileLaunch_Update(void);
void SiloMissile_Explode(void);
void TileEffectTask_Destroy(void);
void TileEffectTask_ScrollToTile(void);
void TileEffectTask_Update(void);
void UnitCrash_ScrollToUnit(void);
void UnitDestroyTask_PlayEffectAndRemove(void);
void UnitMenu_Open(void);
void func_02021b34(void);
void func_020eb434(void);
void func_020eb4e8(void);
void func_020ebb98(void);
void func_020ebecc(void);
void func_020ebf98(void);
void func_020ebfec(void);
void func_020ec04c(void);
void func_020ecb04(void);
extern u8 gMapScrollScript[];
void CaptureCountUp_Start(void);
void CapturePicture_InitOffset(void);

char data_02168d28[] = {
#include "data/main/data_02168d28.inc.c"
};
char data_02168d20[] = {
#include "data/main/data_02168d20.inc.c"
};
char data_02168d24[] = {
#include "data/main/data_02168d24.inc.c"
};
char data_02168d1c[] = {
#include "data/main/data_02168d1c.inc.c"
};
char data_02168d2c[] = {
#include "data/main/data_02168d2c.inc.c"
};
/* file name per army style */
char* data_02168d30[] = {(char*)&data_02168d2c, (char*)&data_02168d2c,
                         (char*)&data_02168d20, (char*)&data_02168d24,
                         (char*)&data_02168d28, (char*)&data_02168d1c};
TaskScriptCmd data_02168d48[] = {
#include "data/main/data_02168d48.inc.c"
};
TaskScriptCmd data_02168d68[] = {
#include "data/main/data_02168d68.inc.c"
};
TaskScriptCmd data_02168d88[] = {
#include "data/main/data_02168d88.inc.c"
};
TaskScriptCmd data_02168da8[] = {
#include "data/main/data_02168da8.inc.c"
};
TaskScriptCmd data_02168dc8[] = {
#include "data/main/data_02168dc8.inc.c"
};
TaskScriptCmd data_02168df0[] = {
#include "data/main/data_02168df0.inc.c"
};
TaskScriptCmd data_02168e18[] = {
#include "data/main/data_02168e18.inc.c"
};
TaskScriptCmd data_02168e48[] = {
#include "data/main/data_02168e48.inc.c"
};
TaskScriptCmd data_02168e78[] = {
#include "data/main/data_02168e78.inc.c"
};
TaskScriptCmd data_02168ea8[] = {
#include "data/main/data_02168ea8.inc.c"
};
TaskScriptCmd data_02168f10[] = {
#include "data/main/data_02168f10.inc.c"
};
TaskScriptCmd gTileEffectTaskScript[] = {
#include "data/main/gTileEffectTaskScript.inc.c"
};
TaskScriptCmd data_02168f48[] = {
#include "data/main/data_02168f48.inc.c"
};
TaskScriptCmd data_02168f90[] = {
#include "data/main/data_02168f90.inc.c"
};
EventScriptCommand data_02168fd8[] = {
#include "data/main/data_02168fd8.inc.c"
};
TaskScriptCmd data_02169028[] = {
#include "data/main/data_02169028.inc.c"
};
TaskScriptCmd data_02169088[] = {
#include "data/main/data_02169088.inc.c"
};
TaskScriptCmd data_021690f8[] = {
#include "data/main/data_021690f8.inc.c"
};
