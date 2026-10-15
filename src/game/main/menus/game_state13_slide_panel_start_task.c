#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"

extern const TaskScriptCmd data_02168030[];

void SidePanel_Place(int mode);
BOOL EventScript_IsRunning(void);

/* Handler for game state 13. Slides the side panel away from the cursor, then starts the task
 * at data_02168030 and switches to state 14. Waits for event scripts to end
 * first, except on maps 224, 225, 227 and 229. */
void Game_State13_SlidePanelStartTask(void) {
  SidePanel_Place(SIDE_PANEL_SLIDE_AWAY);
  switch (MAP_STATE->unk_1c4->mapId) {
    case 224:
    case 225:
    case 227:
    case 229:
      break;
    default:
      if (EventScript_IsRunning()) {
        return;
      }
      break;
  }
  Task_Create(data_02168030, TASK_LIST(3));
  MAP_STATE->unk_18 = 14;
}
