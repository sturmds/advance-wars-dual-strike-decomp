#include "types.h"
#include "game/event_queue.h"
#include "game/menu_state.h"
#include "game/task.h"
#include "game/link_slots.h"
#include "game/link_buffers.h"

u8* Wireless_GetSharedData(int player);
void func_0207b48c(void);
int func_0207bd94(void);
u16 Wireless_GetChildAids(void);
void func_020724f0(void);
void LinkScreen_HandleKeys(void);
void LinkScreen_Cancel(void);
/* Needed to match: these take u8 here (their definitions take int), which
 * decides where mwcc narrows the player to a byte. */
void LinkScreen_MovePlayerUp(u8 player);
void LinkScreen_SetReady(u8 player);
void LinkScreen_ClearReady(u8 player);
void LinkScreen_MovePlayerToPanel(u8 player, int panel);
void LinkScreen_MovePlayerDown(u8 player);
void Task_ResumeScript(Task* task);

/* Task step of the link phase screen. Reads four 2-byte messages (type,
 * argument) from each connected player: 1 sets ready, 2 clears ready (or,
 * from player 0 while not ready, backs out of the screen), 3 moves the
 * player to the panel in the argument, 4 and 5 move it up and down. Once
 * every connected player is ready the screen ends; otherwise this
 * console reads its own keys. */
void LinkPhase_Update(Task* task) {
  int cancel;
  int allReady;
  u8* msg;
  int i;
  int player;

  allReady = 1;
  cancel = 0;
  if (func_0207bd94() == 1) {
    for (player = 0; player < LINK_SLOT_COUNT; player++) {
      if ((Wireless_GetChildAids() | 1) & (1 << player)) {
        msg = Wireless_GetSharedData((u16)player);
        for (i = 0; i < LINK_MESSAGE_COUNT; i++, msg += LINK_MESSAGE_SIZE) {
          switch (msg[0]) {
            case 1:
              LinkScreen_SetReady(player);
              break;
            case 2:
              if (LINK_READY_FLAG(gMenuState, player) != 0) {
                LinkScreen_ClearReady(player);
              } else if (player == 0) {
                cancel = 1;
              }
              break;
            case 3:
              LinkScreen_MovePlayerToPanel(player, msg[1]);
              break;
            case 4:
              LinkScreen_MovePlayerUp(player);
              break;
            case 5:
              LinkScreen_MovePlayerDown(player);
              break;
          }
        }
      }
    }
    EventQueue_Drain();
  }
  if (cancel == 1) {
    LinkScreen_Cancel();
    func_0207b48c();
    Task_ResumeScript(task);
    return;
  }
  for (player = 0; player < LINK_SLOT_COUNT; player++) {
    if ((Wireless_GetChildAids() | 1) & (1 << player)) {
      if (LINK_READY_FLAG(gMenuState, player) == 0) {
        allReady = 0;
        break;
      }
    }
  }
  if (allReady == 1) {
    func_020724f0();
    func_0207b48c();
    Task_ResumeScript(task);
    return;
  }
  LinkScreen_HandleKeys();
}
