#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02167fb8[];

BOOL Map_HasHealObject(void);

/* Starts the data_02167fb8 task when Map_HasHealObject says the map has an object of kind 9 or 10. */
void ObjectHealScan_Start(void) {
  if (Map_HasHealObject()) {
    Task_Create(data_02167fb8, TASK_LIST(3));
  }
}
