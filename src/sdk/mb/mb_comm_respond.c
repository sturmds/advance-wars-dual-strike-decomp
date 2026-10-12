#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

extern DlPlayHostLinkArea* data_022aced4;

/* Answers a child that is waiting for the application: sets its childRequest
 * when the child is in the state that answer applies to. Returns 1 when the
 * request was set, else 0. */
int MbComm_Respond(u32 aid, int response) {
  u32 request;
  u32 expectedState;
  OSIntrMode oldMode;

  oldMode = OS_DisableInterrupts();

  switch (response) {
    case DLPLAY_ANSWER_TURN_AWAY:
      expectedState = DLPLAY_CHILD_ASKED_FILE;
      request = DLPLAY_APP_WANTS_REFUSE;
      break;
    case DLPLAY_ANSWER_ADMIT:
      expectedState = DLPLAY_CHILD_ASKED_FILE;
      request = DLPLAY_APP_WANTS_ADMIT;
      break;
    case DLPLAY_ANSWER_BEGIN_UPLOAD:
      expectedState = DLPLAY_CHILD_AWAITING_START;
      request = DLPLAY_APP_WANTS_UPLOAD;
      break;
    case DLPLAY_ANSWER_START_PROGRAM:
      expectedState = DLPLAY_CHILD_HAS_PROGRAM;
      request = DLPLAY_APP_WANTS_START;
      break;
    default:
      OS_RestoreInterrupts(oldMode);
      return 0;
  }
  if (data_022aced4 != NULL && MbComm_IsChildAid(aid) &&
      data_022aced4->commState[aid - 1] == expectedState) {
    data_022aced4->childRequests[aid - 1] = request;
    OS_RestoreInterrupts(oldMode);
    return 1;
  }
  OS_RestoreInterrupts(oldMode);
  return 0;
}
