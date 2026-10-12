#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/ov003_frame_task.h"
#include "sdk/msl_string.h"

u16 Wireless_GetChildAids(void);
u16* Wireless_GetSharedData(u16 aid);
int func_0207bd94(void);
void func_0207b48c(void);
void func_0207b4b0(void* a0);
void Link_FlagError(void);
void Task_ResumeScript(Ov003FrameTask* task);

extern char data_ov003_02364b44[];

/* Task script command. Outside a link game it goes straight on. In one it
 * waits while strcmp matches some link player against the string
 * "cmfade" (data_ov003_02364b44), counting frames in the task; after 300
 * frames it flags a link error. Same logic as func_02081f6c. */
void Ov003_WaitLinkFade(Ov003FrameTask* task) {
  int i;
  int found;

  if (data_02160ac0->unk_7c == 0) {
    Task_ResumeScript(task);
    return;
  }
  if (!func_0207bd94()) {
    return;
  }
  found = 0;
  for (i = 0; i < 8; i++) {
    if ((Wireless_GetChildAids() | 1) & (1 << i)) {
      if (strcmp((const char*)Wireless_GetSharedData((u16)i),
                 data_ov003_02364b44)) {
        found = 1;
        break;
      }
    }
  }
  if (found == 1) {
    func_0207b4b0(data_ov003_02364b44);
    task->frame++;
    if (task->frame > 300) {
      Link_FlagError();
    }
  } else {
    task->frame = 0;
    func_0207b48c();
    Task_ResumeScript(task);
  }
}
