#include "types.h"
#include "game/task.h"
#include "game/reaction_text_menu_task.h"

ReactionTextMenuTask* Task_FindByScript(const void* script);
void EventSequenceTask_Start(Task* parent);

extern TaskScriptCmd data_0215fc0c[];

/* Starts the event sequence once the name entry for the chosen text has ended. */
void ReactionTextMenu_StartEventWhenDone(Task* parent) {
  if (Task_FindByScript(data_0215fc0c)->done == 0) {
    return;
  }
  EventSequenceTask_Start(parent);
}
