#include "types.h"
#include "game/event_queue.h"
#include "game/task.h"

u8* Wireless_GetSharedData(int player);
void func_0207b48c(void);
int func_0207bd94(void);
void func_020b9864(void);
void func_020b9890(void);
void LinkMenu_Exit(void);
void LinkMenu_PickStage1Button(int button);
void Task_ResumeScript(Task* task);

/* Task step of the link menu. Reads four 2-byte messages (type, argument) from player 0.
 * Type 1 picks stage 1 button <argument> and resumes the script; type 2 leaves the
 * menu and resumes it. */
void LinkMenuTask_HandlePeerMessages(Task* task) {
  u8* msg;
  int i;

  if (func_0207bd94() != 1) {
    return;
  }
  msg = Wireless_GetSharedData(0);
  for (i = 0; i < 4; msg += 2, i++) {
    switch (msg[0]) {
      case 1:
        LinkMenu_PickStage1Button(msg[1]);
        EventQueue_Reset();
        func_0207b48c();
        func_020b9890();
        Task_ResumeScript(task);
        return;
      case 2:
        LinkMenu_Exit();
        EventQueue_Reset();
        func_0207b48c();
        func_020b9864();
        Task_ResumeScript(task);
        return;
    }
  }
  EventQueue_Drain();
}
