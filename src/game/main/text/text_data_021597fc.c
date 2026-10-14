#include "types.h"
#include "game/task.h"
#include "game/mode_font_task.h"

void ModeFontTask_GrowIn(void);
void ModeFontTask_GrowInFramed(void);
void ModeFontTask_InitLetterStarts(void);
void ModeFontTask_Draw(void);
void ModeFontTask_DrawPlain(void);
void ModeFontTask_SlideIn(void);

/* "/ohashi/res_breakfont" */
char data_021597fc[] = {
#include "data/main/data_021597fc.inc.c"
};
u8 data_02159814[] = {
#include "data/main/data_02159814.inc.c"
};
TaskScriptCmd data_02159830[] = {
#include "data/main/data_02159830.inc.c"
};
TaskScriptCmd data_02159870[] = {
#include "data/main/data_02159870.inc.c"
};
