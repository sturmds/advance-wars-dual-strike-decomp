#include "types.h"
#include "game/task.h"

void ResultPoint_DrawUpdate(void);
void ResultPoint_Free(void);
void ResultPoint_ScrollAllUpdate(void);
void ResultPoint_ScrollRowsUpdate(void);
void ResultPoint_SlideInUpdate(void);
void ResultPoint_SlideUpdate(void);
void ResultPoint_SumPoints(void);
void ResultPoint_TallyUpdate(void);
void func_0203833c(void);

u8 data_0215a434[9] = {
#include "data/main/data_0215a434.inc.c"
};
char gCaptionLetters[12] = {
#include "data/main/gCaptionLetters.inc.c"
};
TaskScriptCmd data_0215a44c[13] = {
#include "data/main/data_0215a44c.inc.c"
};
