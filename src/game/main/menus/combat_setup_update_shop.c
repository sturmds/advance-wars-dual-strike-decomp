#include "types.h"
#include "game/event_queue.h"
#include "game/keypad.h"
#include "game/menu_state.h"
#include "game/task.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_0229ff30.h"
#include "game/Unk_02160ac0.h"

u8* Wireless_GetSharedData(int player);
void func_0207b48c(void);
int func_0207bd94(void);
void CombatSetup_FinishShop(void);
void CombatSetup_LeaveShop(void);
void CombatSetup_MarkReady(u8 player);
void CombatSetup_PickShopButton(u8 player, int arg);
void CombatSetup_SendCancel(void);
/* Needed to match: declared with a u8 player (defined with int), so each call
 * narrows the loop counter again. */
void CombatSetup_BuyUnit(u8 player, int button);
void CombatSetup_SellLastUnit(u8 player);
void CombatSetup_CancelReady(u8 player);
void Task_ResumeScript(Task* task);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Task step of the Combat setup shop (link play). Reads four 2-byte messages
 * (type, argument) from each connected player who owns their row: 20 marks
 * the row ready, 21 takes the mark back (or leaves the screen when there is
 * none), 22 buys a unit, 23 sells the last one, 24 takes the mark back, sells,
 * or leaves when the row is empty, 25 goes to CombatSetup_PickShopButton. Once every row in
 * use (rows[].memberCount) is ready, the screen moves on (CombatSetup_FinishShop). Otherwise
 * B from the local row owner on a ready row sends message 24. */
void CombatSetup_UpdateShop(Task* task) {
  u8* msg;
  int leave;
  int j;
  int allReady;
  int i;

  allReady = 1;
  leave = 0;
  if (func_0207bd94() == 1) {
    for (i = 0; i < 8; i++) {
      if (!(data_0229ff30.playerMask & (1 << i))) {
        continue;
      }
      if (i != MENU->combat.rows[MENU->combat.players[i].row - 1].player) {
        continue;
      }
      msg = Wireless_GetSharedData((u16)i);
      for (j = 0; j < 4; msg += 2, j++) {
        switch (msg[0]) {
          case 22:
            CombatSetup_BuyUnit(i, msg[1]);
            break;
          case 23:
            CombatSetup_SellLastUnit(i);
            break;
          case 20:
            CombatSetup_MarkReady(i);
            break;
          case 24:
            if (MENU->combat.rows[MENU->combat.players[i].row - 1].locked !=
                0) {
              CombatSetup_CancelReady(i);
            } else if (LINK_ROWS[MENU->combat.players[i].row - 1].spawnCount ==
                       0) {
              leave = 1;
            } else {
              CombatSetup_SellLastUnit(i);
            }
            break;
          case 21:
            if (MENU->combat.rows[MENU->combat.players[i].row - 1].locked ==
                0) {
              leave = 1;
            } else {
              CombatSetup_CancelReady(i);
            }
            break;
          case 25:
            CombatSetup_PickShopButton(i, msg[1]);
            break;
        }
      }
    }
    EventQueue_Drain();
  }
  if (leave == 1) {
    CombatSetup_LeaveShop();
    func_0207b48c();
    Task_ResumeScript(task);
    return;
  }
  for (i = 1; i < 5; i++) {
    if (MENU->combat.rows[i - 1].memberCount != 0 &&
        MENU->combat.rows[i - 1].locked == 0) {
      allReady = 0;
      break;
    }
  }
  if (allReady == 1) {
    CombatSetup_FinishShop();
    func_0207b48c();
    Task_ResumeScript(task);
    return;
  }
  if (data_0229fb30.unk_45a !=
      MENU->combat.rows[MENU->combat.players[data_0229fb30.unk_45a].row - 1]
          .player) {
    return;
  }
  if (!(gKeypad->pressed & PAD_BUTTON_B)) {
    return;
  }
  if (MENU->combat.rows[MENU->combat.players[data_0229fb30.unk_45a].row - 1]
          .locked == 0) {
    return;
  }
  CombatSetup_SendCancel();
}
