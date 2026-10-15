#include "types.h"
#include "game/reaction_text_menu_task.h"
#include "game/task.h"
#include "sdk/msl_string.h"

ReactionTextMenuTask* Task_FindByScript(const void* script);
int func_020b983c(void);
u8* func_021016e4(void);

extern TaskScriptCmd data_0215fc0c[];

/* Copies the text typed in the name entry into the chosen reaction text. */
void ReactionTextMenu_StoreTyped(void) {
  ReactionTextMenuTask* menu = Task_FindByScript(data_0215fc0c);

  if (func_020b983c() != 1) {
    return;
  }
  strcpy(data_0229da8c.texts[menu->selected], (char*)func_021016e4());
  menu->done = 1;
}
