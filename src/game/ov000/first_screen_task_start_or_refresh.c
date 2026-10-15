#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void* Task_FindByScript(const void* script);
void PlayerPanels_Refresh(void);
extern TaskScriptCmd data_ov000_0234eb64[];

/* On the first screen: calls PlayerPanels_Refresh if the task with script
 * data_ov000_0234eb64 exists, otherwise creates it. */
void FirstScreenTask_StartOrRefresh(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (Task_FindByScript(data_ov000_0234eb64) != NULL) {
    PlayerPanels_Refresh();
  } else {
    Task_Create(data_ov000_0234eb64, TASK_LIST(3));
  }
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
