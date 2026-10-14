#include "types.h"
#include "sdk/os.h"
#include "sdk/msl.h"
#include "sdk/msl_file.h"
#include "sdk/os_common.h"

OSMutex gMslLockMutexes[MSL_LOCK_COUNT];
u8 data_022ad424[FILE_BUFFER_SIZE];
u8 data_022ad224[FILE_BUFFER_SIZE];
u8 data_022ad324[FILE_BUFFER_SIZE];

BOOL OS_TryLockMutex(OSMutex* mutex);
void OS_LockMutex(OSMutex* mutex);
void OS_UnlockMutex(OSMutex* mutex);
void exit(int status);

int raise(int sig) {
  SignalHandler handler;

  if (sig < 1 || sig > SIGNAL_COUNT) {
    return -1;
  }

  if (!OS_TryLockMutex(&gMslLockMutexes[MSL_LOCK_SIGNAL])) {
    gMslLockOwners[MSL_LOCK_SIGNAL] = OSi_ThreadInfo.current->id;
    gMslLockCounts[MSL_LOCK_SIGNAL] = 1;
  } else if (gMslLockOwners[MSL_LOCK_SIGNAL] == OSi_ThreadInfo.current->id) {
    gMslLockCounts[MSL_LOCK_SIGNAL]++;
  } else {
    OS_LockMutex(&gMslLockMutexes[MSL_LOCK_SIGNAL]);
    gMslLockOwners[MSL_LOCK_SIGNAL] = OSi_ThreadInfo.current->id;
    gMslLockCounts[MSL_LOCK_SIGNAL] = 1;
  }

  handler = gMslSignalHandlers[sig - 1];
  if (handler != SIGNAL_HANDLER_IGNORE) {
    gMslSignalHandlers[sig - 1] = SIGNAL_HANDLER_DEFAULT;
  }

  if (--gMslLockCounts[MSL_LOCK_SIGNAL] == 0) {
    OS_UnlockMutex(&gMslLockMutexes[MSL_LOCK_SIGNAL]);
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
