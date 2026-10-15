#include "types.h"
#include "game/ai_profile.h"
#include "game/ai/ai_power_check.h"
#include "game/ai/ai_power.h"

void func_02099520(void);

/* per target group: the unit type that stands for it */
u8 gAiTargetGroupType[] = {
#include "data/main/gAiTargetGroupType.inc.c"
};
AiProfileSet data_02161b8c[] = {
#include "data/main/data_02161b8c.inc.c"
};
AiProfileMods gAiProfileMods[] = {
#include "data/main/gAiProfileMods.inc.c"
};
AiProfileTemplate gAiProfileTemplates[] = {
#include "data/main/gAiProfileTemplates.inc.c"
};
