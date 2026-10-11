#include "types.h"
#include "sdk/os.h"

#define EXIT_TABLE_SIZE 64

typedef void (*AtExitFunction)(void);

extern OSThreadInfo OSi_ThreadInfo;
extern OSMutex gMslExitMutex;
extern u32 data_022ad128;
extern int data_022ad14c;
extern int gMslExitFuncCount;
extern AtExitFunction gMslExitFuncs[EXIT_TABLE_SIZE];
extern AtExitFunction gMslExitHook;

BOOL OS_TryLockMutex(OSMutex *mutex);
void OS_LockMutex(OSMutex *mutex);
void OS_UnlockMutex(OSMutex *mutex);
void func_0212218c(int stream);
void func_021295b4(void);

void MSL_Exit(int status)
{
    /* Branch order follows the original, including setting the owner when the try-lock fails. */
    if (!OS_TryLockMutex(&gMslExitMutex)) {
        data_022ad128 = OSi_ThreadInfo.current->id;
        data_022ad14c = 1;
    } else if (data_022ad128 == OSi_ThreadInfo.current->id) {
        data_022ad14c++;
    } else {
        OS_LockMutex(&gMslExitMutex);
        data_022ad128 = OSi_ThreadInfo.current->id;
        data_022ad14c = 1;
    }

    while (gMslExitFuncCount > 0) {
        gMslExitFuncs[--gMslExitFuncCount]();
    }

    if (--data_022ad14c == 0) {
        OS_UnlockMutex(&gMslExitMutex);
    }

    if (gMslExitHook != NULL) {
        gMslExitHook();
        gMslExitHook = NULL;
    }

    func_0212218c(0);
    func_021295b4();
}
