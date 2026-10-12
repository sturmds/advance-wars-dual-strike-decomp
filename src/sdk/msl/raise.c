#include "types.h"
#include "sdk/os.h"
#include "sdk/msl.h"

#define SIGNAL_COUNT 7
#define SIGNAL_ABORT 1
#define SIGNAL_HANDLER_DEFAULT ((SignalHandler)0)
#define SIGNAL_HANDLER_IGNORE ((SignalHandler)1)

typedef void (*SignalHandler)(int sig);

extern OSThreadInfo OSi_ThreadInfo;
extern OSMutex gMslSignalMutex;
extern SignalHandler gMslSignalHandlers[SIGNAL_COUNT];

BOOL OS_TryLockMutex(OSMutex* mutex);
void OS_LockMutex(OSMutex* mutex);
void OS_UnlockMutex(OSMutex* mutex);
void exit(int status);

int raise(int sig) {
  SignalHandler handler;

  if (sig < 1 || sig > SIGNAL_COUNT) {
    return -1;
  }

  if (!OS_TryLockMutex(&gMslSignalMutex)) {
    gMslLockOwners[MSL_LOCK_SIGNAL] = OSi_ThreadInfo.current->id;
    gMslLockCounts[MSL_LOCK_SIGNAL] = 1;
  } else if (gMslLockOwners[MSL_LOCK_SIGNAL] == OSi_ThreadInfo.current->id) {
    gMslLockCounts[MSL_LOCK_SIGNAL]++;
  } else {
    OS_LockMutex(&gMslSignalMutex);
    gMslLockOwners[MSL_LOCK_SIGNAL] = OSi_ThreadInfo.current->id;
    gMslLockCounts[MSL_LOCK_SIGNAL] = 1;
  }

  handler = gMslSignalHandlers[sig - 1];
  if (handler != SIGNAL_HANDLER_IGNORE) {
    gMslSignalHandlers[sig - 1] = SIGNAL_HANDLER_DEFAULT;
  }

  if (--gMslLockCounts[MSL_LOCK_SIGNAL] == 0) {
    OS_UnlockMutex(&gMslSignalMutex);
  }

  if (handler == SIGNAL_HANDLER_IGNORE ||
      (handler == SIGNAL_HANDLER_DEFAULT && sig == SIGNAL_ABORT)) {
    return 0;
  }
  if (handler == SIGNAL_HANDLER_DEFAULT) {
    exit(0);
  }
  handler(sig);
  return 0;
}
