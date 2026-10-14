#include "types.h"
#include "game/task.h"

void PaletteCycle_Update(void);

u16 gMapPreviewBlockUnitMask[4] = {
#include "data/main/gMapPreviewBlockUnitMask.inc.c"
};
TaskScriptCmd data_02159944[4] = {
#include "data/main/data_02159944.inc.c"
};
