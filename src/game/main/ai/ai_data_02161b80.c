#include "types.h"
#include "game/ai_profile.h"

void AiPowerCheck_EnemiesOnProperties(void);
void AiPowerCheck_HasFactoryOrFreeCity(void);
void AiTurn_IsPhase3(void);
void func_02098e7c(void);
void func_020990c4(void);
void func_020991b0(void);
void func_020991f4(void);
void func_020992c8(void);
void func_02099338(void);
void func_020994b8(void);
void func_02099520(void);

/* per target group: the unit type that stands for it */
u8 gAiTargetGroupType[] = {
#include "data/main/gAiTargetGroupType.inc.c"
};
/* sets of 29 bytes: a mods id, then a template id per CO id */
u8 data_02161b8c[] = {
#include "data/main/data_02161b8c.inc.c"
};
AiProfileMods gAiProfileMods[] = {
#include "data/main/gAiProfileMods.inc.c"
};
AiProfileTemplate gAiProfileTemplates[] = {
#include "data/main/gAiProfileTemplates.inc.c"
};
