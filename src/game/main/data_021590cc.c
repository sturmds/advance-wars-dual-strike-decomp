#include "types.h"
#include "game/task.h"

void HelpBand_Init(void);
void HelpBand_Update(void);

u16 gWhitePalette[] = {
#include "data/main/gWhitePalette.inc.c"
};
TaskScriptCmd data_021590ec[] = {
#include "data/main/data_021590ec.inc.c"
};
