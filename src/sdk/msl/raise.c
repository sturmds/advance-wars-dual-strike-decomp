#include "types.h"
#include "sdk/os.h"

#define SIGNAL_COUNT 7
#define SIGNAL_ABORT 1
#define SIGNAL_HANDLER_DEFAULT ((SignalHandler)0)
#define SIGNAL_HANDLER_IGNORE ((SignalHandler)1)

typedef void (*SignalHandler)(int sig);

extern OSThreadInfo OSi_ThreadInfo;
extern OSMutex gMslSignalMutex;
extern SignalHandler gMslSignalHandlers[SIGNAL_COUNT];
/* Per-lock owner and count tables; MSL_Exit uses entry 0 of the same tables. */
extern u32 data_022ad128[];
extern int data_022ad14c[];

#define SIGNAL_LOCK 7

BOOL OS_TryLockMutex(OSMutex *mutex);
void OS_LockMutex(OSMutex *mutex);
void OS_UnlockMutex(OSMutex *mutex);
void exit(int status);

int raise(int sig)
{
    SignalHandler handler;

    if (sig < 1 || sig > SIGNAL_COUNT) {
        return -1;
    }

    if (!OS_TryLockMutex(&gMslSignalMutex)) {
        data_022ad128[SIGNAL_LOCK] = OSi_ThreadInfo.current->id;
        data_022ad14c[SIGNAL_LOCK] = 1;
    } else if (data_022ad128[SIGNAL_LOCK] == OSi_ThreadInfo.current->id) {
        data_022ad14c[SIGNAL_LOCK]++;
    } else {
        OS_LockMutex(&gMslSignalMutex);
        data_022ad128[SIGNAL_LOCK] = OSi_ThreadInfo.current->id;
        data_022ad14c[SIGNAL_LOCK] = 1;
    }

    handler = gMslSignalHandlers[sig - 1];
    if (handler != SIGNAL_HANDLER_IGNORE) {
        gMslSignalHandlers[sig - 1] = SIGNAL_HANDLER_DEFAULT;
    }

    if (--data_022ad14c[SIGNAL_LOCK] == 0) {
        OS_UnlockMutex(&gMslSignalMutex);
    }

    if (handler == SIGNAL_HANDLER_IGNORE || (handler == SIGNAL_HANDLER_DEFAULT && sig == SIGNAL_ABORT)) {
        return 0;
    }
    if (handler == SIGNAL_HANDLER_DEFAULT) {
        exit(0);
    }
    handler(sig);
    return 0;
}
