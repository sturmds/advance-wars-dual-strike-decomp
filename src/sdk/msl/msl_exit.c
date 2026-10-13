#include "types.h"
#include "sdk/os.h"
#include "sdk/msl.h"
#include "sdk/msl_file.h"

#define EXIT_TABLE_SIZE 64

typedef void (*AtExitFunction)(void);

extern OSThreadInfo OSi_ThreadInfo;
extern OSMutex gMslExitMutex;
extern int gMslExitFuncCount;
extern AtExitFunction gMslExitFuncs[EXIT_TABLE_SIZE];
static AtExitFunction gMslExitHook;

BOOL OS_TryLockMutex(OSMutex* mutex);
void OS_LockMutex(OSMutex* mutex);
void OS_UnlockMutex(OSMutex* mutex);
void func_021295b4(void);

void MSL_Exit(int status) {
  /* Branch order follows the original, including setting the owner when the try-lock fails. */
  if (!OS_TryLockMutex(&gMslExitMutex)) {
    gMslLockOwners[MSL_LOCK_EXIT] = OSi_ThreadInfo.current->id;
    gMslLockCounts[MSL_LOCK_EXIT] = 1;
  } else if (gMslLockOwners[MSL_LOCK_EXIT] == OSi_ThreadInfo.current->id) {
    gMslLockCounts[MSL_LOCK_EXIT]++;
  } else {
    OS_LockMutex(&gMslExitMutex);
    gMslLockOwners[MSL_LOCK_EXIT] = OSi_ThreadInfo.current->id;
    gMslLockCounts[MSL_LOCK_EXIT] = 1;
  }

  while (gMslExitFuncCount > 0) {
    gMslExitFuncs[--gMslExitFuncCount]();
  }

  if (--gMslLockCounts[MSL_LOCK_EXIT] == 0) {
    OS_UnlockMutex(&gMslExitMutex);
  }

  if (gMslExitHook != NULL) {
    gMslExitHook();
    gMslExitHook = NULL;
  }

  fflush(NULL);
  func_021295b4();
}
