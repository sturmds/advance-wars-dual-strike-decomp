#include "types.h"
#include "sdk/mb.h"

extern DlPlayHostLinkArea* data_022aced4;

/* Records a child's new comm state (for a child aid) and tells the callback. */
void MbComm_SetState(u32 aid, u32 state, void* arg) {
  if (MbComm_IsChildAid(aid)) {
    data_022aced4->commState[aid - 1] = state;
  }
  MbComm_NotifyState(aid, state, arg);
}
