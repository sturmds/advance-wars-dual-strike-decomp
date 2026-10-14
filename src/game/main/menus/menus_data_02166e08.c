#include "types.h"
#include "game/task.h"

void MapPreviewTask_Update(void);
void func_020d426c(void);
void func_020d4278(void);
void func_020d4288(void);

u8 data_02166e08[] = {
#include "data/main/data_02166e08.inc.c"
};
u16 gMapPreviewUnitMask[] = {
#include "data/main/gMapPreviewUnitMask.inc.c"
};
TaskScriptCmd data_02166e14[] = {
#include "data/main/data_02166e14.inc.c"
};
/* per terrain id: first picture of the terrain in data_021587e0 */
u16 data_02166e3c[] = {
#include "data/main/data_02166e3c.inc.c"
};
