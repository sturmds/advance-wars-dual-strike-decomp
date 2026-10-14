#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/link_buffers.h"
#include "game/event_queue.h"
#include "game/menu_state.h"
#include "game/task.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_0229ff30.h"

u8* Wireless_GetSharedData(int player);
void func_0207b48c(void);
int func_0207bd94(void);
void func_020858bc(void);
void func_020858d8(void);
void CombatSetup_SetPlayerUnready(u8 player);
void CombatSetup_SetPlayerReady(u8 player);
void CombatSetup_MovePlayerDown(u8 player);
void CombatSetup_MovePlayerUp(u8 player);
void CombatSetup_MoveToNextRow(u8 player);
void CombatSetup_MoveToPrevRow(u8 player);
void CombatSetup_MovePlayerToRow(u8 player, u8 arg);
void CombatSetup_HandleRowKeys(void);
void Task_ResumeScript(Task* task);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Task step of the Combat setup screen (link play). Reads four 2-byte
 * messages (type, argument) from each connected player: 9 marks the player
 * ready, 10 not ready (from player 0 while not ready: leave the screen),
 * 11-15 go to other handlers. Then, once every connected player is ready
 * and they fill at least two team rows, the screen ends; otherwise
 * CombatSetup_HandleRowKeys handles the local player's keys.
 * MenuState.combat.players[].ready holds the ready flag. */
void CombatSetup_UpdateLink(Task* task) {
  u8* msg;
  int allReady;
  u8 teams;
  int j;
  int cancel;
  u8 rowMask;
  int i;

  cancel = 0;
  rowMask = 0;
  allReady = 1;
  teams = 0;

  if (func_0207bd94() == 1) {
    for (i = 0; i < LINK_AID_COUNT; i++) {
      if (data_0229ff30.playerMask & (1 << i)) {
        msg = Wireless_GetSharedData((u16)i);
        for (j = 0; j < LINK_MESSAGE_COUNT; msg += 2, j++) {
          switch (msg[0]) {
            case 9:
              CombatSetup_SetPlayerReady(i);
              break;
            case 10:
              if (MENU->combat.players[i].ready != 0) {
                CombatSetup_SetPlayerUnready(i);
              } else if (i == 0) {
                cancel = 1;
              }
              break;
            case 11:
              CombatSetup_MovePlayerToRow(i, msg[1]);
              break;
            case 12:
              CombatSetup_MoveToPrevRow(i);
              break;
            case 13:
              CombatSetup_MoveToNextRow(i);
              break;
            case 14:
              CombatSetup_MovePlayerUp(i);
              break;
            case 15:
              CombatSetup_MovePlayerDown(i);
              break;
          }
        }
      }
    }
    EventQueue_Drain();
  }
  if (cancel == 1) {
    func_020858d8();
    func_0207b48c();
    Task_ResumeScript(task);
    return;
  }
  for (i = 0; i < LINK_AID_COUNT; i++) {
    if (data_0229ff30.playerMask & (1 << i)) {
      if (MENU->combat.players[i].ready == 0) {
        allReady = 0;
        break;
      }
      rowMask |= 1 << MENU->combat.players[i].row;
    }
  }
  if (allReady == 1) {
    for (i = 1; i < COMBAT_ROW_COUNT + 1; i++) {
      if (rowMask & (1 << i)) {
        teams++;
      }
    }
    if (teams >= 2) {
      func_020858bc();
      func_0207b48c();
      Task_ResumeScript(task);
      return;
    }
  }
  CombatSetup_HandleRowKeys();
}
