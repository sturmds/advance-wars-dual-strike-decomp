#include "types.h"

extern int gMslSkipShutdown;
extern void (*gMslShutdownHook)(void);

void func_021296b8(void);
void MSL_Exit(int status);

void exit(int status)
{
    if (gMslSkipShutdown == 0) {
        func_021296b8();
        if (gMslShutdownHook != NULL) {
            gMslShutdownHook();
            gMslShutdownHook = NULL;
        }
    }
    MSL_Exit(status);
}
