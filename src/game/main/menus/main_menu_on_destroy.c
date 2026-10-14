#include "types.h"
#include "game/main_menu.h"
#include "game/task.h"

void HelpBand_Stop(void);
void Wallpaper_DestroyTasks(void);
void Screen1_DestroyTextTasks(void);
int func_0202d874(void);
int Task_DestroyByScript(const void* script);
void Mem_Free(void* block);

extern TaskScriptCmd data_02159a5c[];

/* Cleans up the main menu: stops the help band, the wallpaper and the
 * text tasks, then frees the page and the work block. */
void MainMenu_OnDestroy(void) {
  HelpBand_Stop();
  Wallpaper_DestroyTasks();
  Screen1_DestroyTextTasks();
  func_0202d874();
  Task_DestroyByScript(data_02159a5c);
  Mem_Free(*data_0229c71c);
  Mem_Free(data_0229c71c);
}
