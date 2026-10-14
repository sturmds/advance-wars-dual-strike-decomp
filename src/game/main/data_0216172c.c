#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void CombatManual_Init(void);
void Screen1_DestroyTextTasks(void);
void Wallpaper_DestroyTasks(void);
void func_0208b9d4(void);
void func_0208ba5c(void);
void func_0208ba60(void);
void func_0208ba10(void);

/* "/ohashi/res_result_lion" */
char gResultLionResPath[24] = {
#include "data/main/gResultLionResPath.inc.c"
};
MenuButtonTemplate data_02161744 = {
#include "data/main/data_02161744.inc.c"
};
TaskScriptCmd data_0216176c[6] = {
#include "data/main/data_0216176c.inc.c"
};
TaskScriptCmd data_0216179c[7] = {
#include "data/main/data_0216179c.inc.c"
};
