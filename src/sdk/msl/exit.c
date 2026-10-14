#include "types.h"
#include "sdk/msl.h"

static void (*gMslShutdownHook)(void);
static int gMslSkipShutdown;
int gMslExitFuncCount;
AtExitFunction gMslExitFuncs[EXIT_TABLE_SIZE];

void __destroy_global_chain(void);
void MSL_Exit(int status);

void exit(int status) {
  if (gMslSkipShutdown == 0) {
    __destroy_global_chain();
    if (gMslShutdownHook != NULL) {
      gMslShutdownHook();
      gMslShutdownHook = NULL;
    }
  }
  MSL_Exit(status);
}
