#include "types.h"
#include "game/Ov003Fx.h"
#include "game/ov003_unit.h"
#include "game/task.h"

void Ov003_InitLayerFade(void);
void Ov003_PointTaskUpdate(void);
void func_ov003_023575fc(void);
void func_ov003_0235ce7c(void);
void func_ov003_0235dd38(void);
void Ov003_StepCoPowerBanner(void);
void Ov003_StepLettersGrow(void);
void Ov003_StepScatterSprites(void);
void Ov003_FadeEvyStep(void);
void Ov003_StepLettersShrink(void);
void func_ov003_0235cb08(void);

Ov003Fx* data_ov003_02364a84 = &data_ov003_02365d00;
Ov003Map* data_ov003_02364a88 = &data_ov003_02368110;
TaskScriptCmd data_ov003_02364a8c[] = {
#include "data/ov003/data_ov003_02364a8c.inc.c"
};
TaskScriptCmd data_ov003_02364aac[] = {
#include "data/ov003/data_ov003_02364aac.inc.c"
};
TaskScriptCmd data_ov003_02364acc[] = {
#include "data/ov003/data_ov003_02364acc.inc.c"
};
