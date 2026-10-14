#include "types.h"
#include "game/script_slot.h"
#include "game/task.h"

void Layer0Picture_Load(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void PowerDamage_BlendIn(void);
void PowerDamage_BlendOut(void);
void PowerDamage_HealNextUnit(void);
void PowerDamage_HitNextEnemy(void);
void PowerDamage_SetWeather(void);
void PowerDamage_StartBlend(void);
void PowerHitEffectTask_Play(void);
extern ScriptCommand data_02166d18[];
void func_020e36d8(void);
void func_020e37c0(void);
void func_020e3dfc(void);
void func_020e3e14(void);
void func_020e3e2c(void);
void BgLayer0_NudgeScroll(void);

TaskScriptCmd gPowerEffectTaskDesc[] = {
#include "data/main/gPowerEffectTaskDesc.inc.c"
};
TaskScriptCmd data_021688dc[] = {
#include "data/main/data_021688dc.inc.c"
};
TaskScriptCmd gPowerDamageTaskDesc[] = {
#include "data/main/gPowerDamageTaskDesc.inc.c"
};
