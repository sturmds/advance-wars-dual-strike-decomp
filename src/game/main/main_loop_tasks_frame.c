#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;

void EventScript_UpdateAll(void);
void ScriptSlot_RunAll(void);
void Task_RunList(Task* task);

/* Main loop step that runs the event scripts, the script slots and task
 * lists 1, 2, 3, 5 and 4, without MainLoop_MapFrame's map state frame. */
void MainLoop_TaskFrame(void) {
  EventScript_UpdateAll();
  ScriptSlot_RunAll();
  Task_RunList(data_027c027c->taskLists[1]);
  Task_RunList(data_027c027c->taskLists[2]);
  Task_RunList(data_027c027c->taskLists[3]);
  Task_RunList(data_027c027c->taskLists[5]);
  Task_RunList(data_027c027c->taskLists[4]);
}
