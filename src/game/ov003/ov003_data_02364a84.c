#include "types.h"
#include "game/Ov003Fx.h"
#include "game/ov003_unit.h"
#include "game/task.h"

void Ov003_InitLayerFade(void);
void Ov003_PointTaskUpdate(void);
extern u8 data_ov003_02365d00[];
extern u8 data_ov003_02368110[];
void func_ov003_023575fc(void);
void func_ov003_0235ce7c(void);
void func_ov003_0235dd38(void);
void Ov003_StepCoPowerBanner(void);
void Ov003_StepLettersGrow(void);
void Ov003_StepScatterSprites(void);
void Ov003_FadeEvyStep(void);
void Ov003_StepLettersShrink(void);
void func_ov003_0235cb08(void);

Ov003Map* data_ov003_02364a88 = (Ov003Map*)&data_ov003_02368110;
Ov003Fx* data_ov003_02364a84 = (Ov003Fx*)&data_ov003_02365d00;
/* declared `u8 data_ov003_02364aac[]` in func_ov003_0235ca24.c: relocation at data_ov003_02364aac+0xc is not on a 4-byte field */
u32 data_ov003_02364aac[8] = {
#include "data/ov003/data_ov003_02364aac.inc.c"
};
TaskScriptCmd data_ov003_02364a8c[4] = {
#include "data/ov003/data_ov003_02364a8c.inc.c"
};
/* declared `u8 data_ov003_02364acc[]` in func_ov003_0235ca24.c: relocation at data_ov003_02364acc+0xc is not on a 4-byte field */
u32 data_ov003_02364acc[9] = {
#include "data/ov003/data_ov003_02364acc.inc.c"
};
u32 data_ov003_02364af0[9] = {
#include "data/ov003/data_ov003_02364af0.inc.c"
};
