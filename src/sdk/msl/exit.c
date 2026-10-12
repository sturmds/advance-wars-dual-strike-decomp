#include "types.h"

extern int gMslSkipShutdown;
extern void (*gMslShutdownHook)(void);

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
