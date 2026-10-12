#include "types.h"
#include "sdk/mb.h"

int MbParent_ResetWm(void);

/* Callback of the task thread's end task (DownloadPlay_RequestWmReset): resets WM. */
void MbParent_ResetWmCallback(DlPlayWorkerJob *info)
{
    MbParent_ResetWm();
}
