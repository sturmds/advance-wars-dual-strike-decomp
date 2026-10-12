#include "types.h"
#include "game/event_queue.h"
#include "game/menu_state.h"
#include "game/task.h"

extern MenuState gMenuState;

u8* Wireless_GetSharedData(u16 player);
u16 Wireless_GetChildAids(void);
void func_0207b48c(void);
int func_0207bd94(void);
void func_0207dcfc(void);
void func_020b9864(void);
void func_020b9890(void);
/* Needed to match: declared with u8 parameters here; the definitions take int. */
void LinkMenu_PickButton(u8 player, u8 button);
void LinkMenu_FinishPick(u8 player);
void LinkMenu_EnterStage1(void);
void LinkMenu_CheckBackKey(void);
void Task_ResumeScript(Task* task);

/* Task step of the two-player link menu in stages 2 and 3. Reads four 2-byte events
 * (type, argument) from each linked player: type 3 picks stage 2 button <argument>,
 * type 4 (back) takes back the player's pick, or, from player 0 with no pick, returns
 * everyone to stage 1. When both players have picked, closes the buttons and resumes
 * the script; otherwise B queues a back event. */
void LinkMenuTask_HandlePicks(Task* task) {
  u8* msg;
  u8 back;
  int i;
  int player;
  u8 allPicked;

  allPicked = 1;
  back = 0;
  if (func_0207bd94() == 1) {
    for (player = 0; player < 4; player++) {
      if ((Wireless_GetChildAids() | 1) & (1 << player)) {
        msg = Wireless_GetSharedData((u16)player);
        for (i = 0; i < 4; i++, msg += 2) {
          switch (msg[0]) {
            case 3: /* pick */
              LinkMenu_PickButton(player, msg[1]);
              break;
            case 4: /* back */
              if (((u8*)&gMenuState + player)[LINK_PICKED_OFFSET]) {
                LinkMenu_FinishPick(player);
              } else if (player == 0) {
                back = 1;
              }
              break;
          }
        }
      }
    }
    EventQueue_Drain();
  }
  if (back == 1) {
    LinkMenu_EnterStage1();
    func_0207b48c();
    for (i = 0; i < 2; i++) {
      ((u8*)&gMenuState + i)[LINK_PICKED_OFFSET] = 0;
    }
    func_020b9864();
    Task_ResumeScript(task);
    return;
  }
  for (i = 0; i < 2; i++) {
    if ((Wireless_GetChildAids() | 1) & (1 << i)) {
      if (!((u8*)&gMenuState + i)[LINK_PICKED_OFFSET]) {
        allPicked = 0;
        break;
      }
    }
  }
  if (allPicked == 1) {
    func_0207dcfc();
    func_0207b48c();
    for (i = 0; i < 2; i++) {
      ((u8*)&gMenuState + i)[LINK_PICKED_OFFSET] = 0;
    }
    func_020b9890();
    Task_ResumeScript(task);
    return;
  }
  LinkMenu_CheckBackKey();
}
