#include "types.h"
#include "game/task.h"

void HelpBand_Init(void);
void HelpBand_Update(void);

u16 gWhitePalette[16] = {
#include "data/main/gWhitePalette.inc.c"
};
TaskScriptCmd data_021590ec[5] = {
#include "data/main/data_021590ec.inc.c"
};
