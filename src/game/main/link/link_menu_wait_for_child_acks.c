#include "types.h"
#include "game/link_wait_task.h"
#include "game/link_buffers.h"

#define LINK_UNFLAGGED_ACK 0x8f

u8* Link_ReadUnflaggedPacket(u8 player);
u16 Wireless_GetChildAids(void);
int func_02077f20(void);
void func_020b9890(void);
void Link_FlagError(void);
void Task_ResumeScript(LinkWaitTask* task);

/* Task step of a player list screen script. Collects an ack (0x8f) from each child and resumes the
 * script once the acks match the connected children (Wireless_GetChildAids). Flags a link error after
 * 300 frames without that. */
void LinkMenu_WaitForChildAcks(LinkWaitTask* task) {
  int aid;
  u8* packet;

  for (aid = 1; aid < LINK_AID_COUNT; aid++) {
    while ((packet = Link_ReadUnflaggedPacket(aid)) != NULL) {
      if (packet[0] == LINK_UNFLAGGED_ACK) {
        task->ackedAids |= 1 << aid;
      }
    }
  }
  if (task->ackedAids == Wireless_GetChildAids() && func_02077f20() == 0) {
    func_020b9890();
    Task_ResumeScript(task);
    return;
  }
  task->waitFrames++;
  if (task->waitFrames >= 300) {
    Link_FlagError();
  }
}
