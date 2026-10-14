#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"
#include "game/Unk_02290118.h"
#include "game/event_script.h"
#include "game/task.h"

extern Unk_0201d320 data_022977f4[];
extern EventSlotHold data_022977f8[];
void EventChoice_Init(void);
void EventChoice_Update(void);

TaskScriptCmd data_02158548[] = {
#include "data/main/data_02158548.inc.c"
};
EventScriptCommand data_02158570[] = {
#include "data/main/data_02158570.inc.c"
};
extern u8 data_02297c2e[];
extern u8 gEventTextMap[];

void EventTextMap_QueueCopy(void);
void EventHold_ReleaseOnTextMapTaskEnd(EventScriptSlot* slot);

/* Event script command: starts the task data_02158548 in task list 3, with the text map
 * buffer gEventTextMap, its upload function (EventTextMap_QueueCopy), the palette slot
 * (data_022977a4.unk_00) and data_02290118.unk_00. The script is held until the task
 * ends, and the slot's run ends for this frame. */
BOOL EventCmd_StartTextMapTask(s16 index) {
  Task* task = Task_Create(data_02158548, TASK_LIST(3));

  /* a word over unk_34 and unk_36, which other task scripts use as halfwords */
  *(void**)&task->unk_34 = data_02297c2e;
  task->unk_38 = gEventTextMap;
  task->unk_3c = EventTextMap_QueueCopy;
  task->unk_40 = data_022977a4.unk_00 << 12;
  task->waitFrames = data_02290118.unk_00;
  data_022977f8[index].hold = (u32)EventHold_ReleaseOnTextMapTaskEnd;
  data_022977f4[index].unk_00++;
  return 0;
}
