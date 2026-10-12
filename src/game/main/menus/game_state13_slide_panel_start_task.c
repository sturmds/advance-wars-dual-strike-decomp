#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern const TaskScriptCmd data_02168030[];

#define STATE (data_027c027c->unk_48[data_027c0284])

void SidePanel_Place(int mode);
BOOL EventScript_IsRunning(void);

/* Handler for game state 13. Slides the side panel away from the cursor, then starts the task
 * at data_02168030 and switches to state 14. Waits for event scripts to end
 * first, except on maps 224, 225, 227 and 229. */
void Game_State13_SlidePanelStartTask(void) {
  SidePanel_Place(SIDE_PANEL_SLIDE_AWAY);
  switch (STATE->unk_1c4->mapId) {
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
  STATE->unk_18 = 14;
}
