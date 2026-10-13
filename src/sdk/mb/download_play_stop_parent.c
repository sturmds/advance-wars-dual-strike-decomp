#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

int DownloadPlay_RequestWmReset(void);

/* Stops Download Play on the parent. Halts the program when MB was never started. */
void DownloadPlay_StopParent(void) {
  OSIntrMode enabled = OS_DisableInterrupts();

  if (data_022aced4->unk_1320 == 0) {
    OS_Terminate();
  }
  DownloadPlay_RequestWmReset();
  OS_RestoreInterrupts(enabled);
}
