#include "types.h"
#include "game/Unk_0229ff30.h"
#include "game/combat_setup_task.h"
#include "game/task.h"
#include "sdk/msl_string.h"

extern char data_02160ae8[];

u16* Wireless_GetSharedData(u16 aid);
int func_0207bd94(void);
void func_0207b48c(void);
void func_0207b4b0(void* a0);
void Link_FlagError(void);
void Task_ResumeScript(Task* task);

/* Task step: while a link player in playerMask still has the data_02160ae8 message pending,
 * resend it and count frames, and flag a link error after 300 frames. Otherwise resume the script. */
void func_02088b3c(CombatSetupTask* task) {
  int i;
  int found;

  if (!func_0207bd94()) {
    return;
  }
  found = 0;
  for (i = 0; i < 8; i++) {
    if (data_0229ff30.playerMask & (1 << i)) {
      if (strcmp((const char*)Wireless_GetSharedData((u16)i), data_02160ae8)) {
        found = 1;
        break;
      }
    }
  }
  if (found == 1) {
    func_0207b4b0(data_02160ae8);
    task->waitFrames++;
    if (task->waitFrames > 300) {
      Link_FlagError();
    }
  } else {
    task->waitFrames = 0;
    func_0207b48c();
    Task_ResumeScript((Task*)task);
  }
}
