#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

/* aligned to 32: no symbol covers the bytes before it */
__attribute__((aligned(32))) DlPlayWorkerArea* data_022acf00;

/* Posts the end task, if the task thread was started: once the tasks before
 * it have run, the thread runs callback and stops. */
void MbTask_End(void (*callback)(DlPlayWorkerJob* info)) {
  OSIntrMode enabled = OS_DisableInterrupts();

  if (MbTask_IsAvailable()) {
    MbTask_Set(&data_022acf00->endTask, NULL, callback, 0);
  }
  OS_RestoreInterrupts(enabled);
}
