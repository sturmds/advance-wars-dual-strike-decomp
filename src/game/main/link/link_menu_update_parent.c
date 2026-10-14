#include "types.h"
#include "game/link_wait_task.h"
#include "game/link_buffers.h"

void LinkMenu_DrawSecondView(void);
u8* Link_ReadUnflaggedPacket(int player);
int func_02077f20(void);
void func_0207bb48(int start);

/* Parent side of the player list task. Discards the children's unflagged packets. Every 4th frame,
 * unless func_02077f20 is nonzero, it calls func_0207bb48(1). */
void LinkMenu_UpdateParent(LinkWaitTask* task) {
  int aid;

  LinkMenu_DrawSecondView();
  for (aid = 1; aid < LINK_AID_COUNT; aid++) {
    while (Link_ReadUnflaggedPacket(aid & 0xff) != NULL) {
    }
  }
  if (task->waitFrames & 3) {
    return;
  }
  if (func_02077f20() != 0) {
    return;
  }
  func_0207bb48(1);
}
