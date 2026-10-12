#include "types.h"
#include "sdk/mb.h"

BOOL func_020fc558(void);

/* Comm callback of the download play parent (see MbComm_SetCallback): answers
 * a child at the states where the application has to. */
void func_020fc700(u32 aid, u32 state, void* arg) {
  switch (state) {
    case DLPLAY_CHILD_HAS_PROGRAM:
      MbComm_Respond(aid, DLPLAY_ANSWER_START_PROGRAM);
      break;
    case DLPLAY_CHILD_ASKED_FILE:
      MbComm_Respond(aid, DLPLAY_ANSWER_ADMIT);
      break;
    case DLPLAY_CHILD_FINISHED:
      func_020fc558();
      break;
    case DLPLAY_CHILD_AWAITING_START:
      MbComm_Respond(aid, DLPLAY_ANSWER_BEGIN_UPLOAD);
      break;
  }
}
