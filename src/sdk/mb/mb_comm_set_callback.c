#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

/* Sets the function that MbComm_NotifyState calls on comm events. */
void MbComm_SetCallback(DlPlayLinkEventFunc callback) {
  OSIntrMode oldMode = OS_DisableInterrupts();

  data_022aced4->callback = callback;
  OS_RestoreInterrupts(oldMode);
}
