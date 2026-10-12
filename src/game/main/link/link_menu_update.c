#include "types.h"
#include "game/event_queue.h"
#include "game/task.h"
#include "game/Unk_0229fb30.h"

u8* Wireless_GetSharedData(int player);
void func_0207b48c(void);
int func_0207bd94(void);
void LinkMenu_SelectEntry(int entry);
void LinkMenu_ApplyCancel(void);
void LinkMenu_ApplyConfirm(void);
void LinkMenu_DecreaseSetting(void);
void LinkMenu_IncreaseSetting(void);
void LinkMenu_HandleKeys(void);
void Task_ResumeScript(Task* task);

/* Task step of the link menu. Reads four 2-byte messages (type, argument)
 * from player 0, who runs the menu: 4 confirms (LinkMenu_ApplyConfirm), 5 cancels
 * (LinkMenu_ApplyCancel), 6 selects an entry, 7 and 8 raise and lower its
 * setting. Without either, player 0's own console handles the keys. */
void LinkMenu_Update(Task* task) {
  int confirm = 0;
  int cancel = 0;
  u8* msg;
  int i;

  if (func_0207bd94() == 1) {
    msg = Wireless_GetSharedData(0);
    for (i = 0; i < 4; i++, msg += 2) {
      switch (msg[0]) {
        case 4:
          confirm = 1;
          break;
        case 5:
          cancel = 1;
          break;
        case 6:
          LinkMenu_SelectEntry(msg[1]);
          break;
        case 7:
          LinkMenu_IncreaseSetting();
          break;
        case 8:
          LinkMenu_DecreaseSetting();
          break;
      }
    }
    EventQueue_Drain();
  }
  if (confirm == 1) {
    LinkMenu_ApplyConfirm();
    EventQueue_Reset();
    func_0207b48c();
    Task_ResumeScript(task);
    return;
  }
  if (cancel == 1) {
    LinkMenu_ApplyCancel();
    EventQueue_Reset();
    func_0207b48c();
    Task_ResumeScript(task);
    return;
  }
  if (data_0229fb30.unk_45a == 0) {
    LinkMenu_HandleKeys();
  }
}
