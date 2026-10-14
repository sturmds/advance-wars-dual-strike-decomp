/* Needed to match: with loop-invariant code motion on, mwcc hoists the u8
 * player for PrevCo and NextCo ahead of the row base and swaps their
 * registers. */
#pragma opt_loop_invariants off

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
/* Needed to match: declared with a u8 player (defined with int), so each call
 * narrows the loop counter again. */
void CombatSetup_LockRow(u8 player);
void CombatSetup_UnlockRow(u8 player);
void CombatSetup_PrevCo(int player);
void CombatSetup_NextCo(int player);
void CombatSetup_FinishCoPick(void);
void CombatSetup_CancelCoPick(void);
void CombatSetup_HandleCoPickKeys(void);
void Task_ResumeScript(Task* task);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Task step of the Combat setup CO pick (link play). Reads four 2-byte
 * messages from each connected player who owns their row: 16 locks the row,
 * 17 unlocks it (or leaves the screen when it is not locked), 18 and 19 pick
 * the previous and next CO. Once every row in use is locked, the pick ends;
 * otherwise the local row owner's keys are handled. */
void CombatSetup_UpdateCoPick(Task* task) {
  u8* msg;
  int leave;
  int j;
  int allLocked;
  int i;

  allLocked = 1;
  leave = 0;
  if (func_0207bd94() == 1) {
    for (i = 0; i < LINK_AID_COUNT; i++) {
      if (!(data_0229ff30.playerMask & (1 << i))) {
        continue;
      }
      if (i != MENU->combat.rows[MENU->combat.players[i].row - 1].player) {
        continue;
      }
      msg = Wireless_GetSharedData((u16)i);
      for (j = 0; j < LINK_MESSAGE_COUNT; j++, msg += 2) {
        switch (msg[0]) {
          case 16:
            CombatSetup_LockRow(i);
            break;
          case 17:
            if (MENU->combat.rows[MENU->combat.players[i].row - 1].locked !=
                0) {
              CombatSetup_UnlockRow(i);
            } else {
              leave = 1;
            }
            break;
          case 18:
            CombatSetup_PrevCo((u8)i);
            break;
          case 19:
            CombatSetup_NextCo((u8)i);
            break;
        }
      }
    }
    EventQueue_Drain();
  }
  if (leave == 1) {
    CombatSetup_CancelCoPick();
    func_0207b48c();
    Task_ResumeScript(task);
    return;
  }
  for (i = 1; i < COMBAT_ROW_COUNT + 1; i++) {
    if (MENU->combat.rows[i - 1].memberCount != 0 &&
        MENU->combat.rows[i - 1].locked == 0) {
      allLocked = 0;
      break;
    }
  }
  if (allLocked == 1) {
    CombatSetup_FinishCoPick();
    func_0207b48c();
    Task_ResumeScript(task);
    return;
  }
  if (data_0229fb30.unk_45a !=
      MENU->combat.rows[MENU->combat.players[data_0229fb30.unk_45a].row - 1]
          .player) {
    return;
  }
  CombatSetup_HandleCoPickKeys();
}
