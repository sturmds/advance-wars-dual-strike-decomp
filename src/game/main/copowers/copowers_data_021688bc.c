#include "types.h"
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
extern u8 data_02166d18[];
void func_020e36d8(void);
void func_020e37c0(void);
void func_020e3dfc(void);
void func_020e3e14(void);
void func_020e3e2c(void);
void BgLayer0_NudgeScroll(void);

/* declared `u8 gPowerEffectTaskDesc[]` in black_storm.c: relocation at gPowerEffectTaskDesc+0xc is not on a 4-byte field */
u32 gPowerEffectTaskDesc[8] = {
#include "data/main/gPowerEffectTaskDesc.inc.c"
};
TaskScriptCmd data_021688dc[5] = {
#include "data/main/data_021688dc.inc.c"
};
TaskScriptCmd gPowerDamageTaskDesc[16] = {
#include "data/main/gPowerDamageTaskDesc.inc.c"
};
