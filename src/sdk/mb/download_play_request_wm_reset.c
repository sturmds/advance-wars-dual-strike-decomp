#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

extern DlPlayStationArea *data_022aced8;

int MbParent_ResetWm(void);
void func_0211c090(void);
int func_0211dd18(void);
void func_0211db08(void (*callback)(void));

/* Ends the parent: WM goes back to the idle state. Returns 1 when this was already
 * requested, else 0 or the WM error code. With the task queue active the reset runs
 * from the queue. */
int DownloadPlay_RequestWmReset(void)
{
    int result = 1;
    OSIntrMode enabled = OS_DisableInterrupts();

    if (data_022aced8->unk_526 == 0) {
        data_022aced8->unk_5e4 = 0;
        data_022aced8->unk_526 = 1;
        if (func_0211dd18()) {
            func_0211db08(func_0211c090);
            result = 0;
        } else {
            result = MbParent_ResetWm();
        }
    }
    OS_RestoreInterrupts(enabled);
    return result;
}
