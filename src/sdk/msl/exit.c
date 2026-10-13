#include "types.h"

static void (*gMslShutdownHook)(void);
static int gMslSkipShutdown;
int gMslExitFuncCount;
/* declared in src/sdk/msl/msl_exit.c, with a type no header defines */
u8 gMslExitFuncs[0x100];

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
