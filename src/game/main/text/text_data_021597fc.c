#include "types.h"
#include "game/mode_font_task.h"

void ModeFontTask_GrowIn(void);
void ModeFontTask_GrowInFramed(void);
void ModeFontTask_InitLetterStarts(void);
void ModeFontTask_Draw(void);
void ModeFontTask_DrawPlain(void);
void ModeFontTask_SlideIn(void);

/* "/ohashi/res_breakfont" */
char data_021597fc[22] = {
#include "data/main/data_021597fc.inc.c"
};
u8 data_02159814[28] = {
#include "data/main/data_02159814.inc.c"
};
/* declared `u8 data_02159830[]` in func_0202a7c0.c: relocation at data_02159830+0x14 is not on a 4-byte field */
u32 data_02159830[16] = {
#include "data/main/data_02159830.inc.c"
};
/* declared `u8 data_02159870[]` in func_0202a7c0.c: relocation at data_02159870+0x14 is not on a 4-byte field */
u32 data_02159870[16] = {
#include "data/main/data_02159870.inc.c"
};
