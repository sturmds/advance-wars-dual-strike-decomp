#ifndef SDK_OS_H
#define SDK_OS_H

#include "types.h"

/* Disables interrupts and halts the CPU for good. */
void OS_Terminate(void);

/* Turn interrupts off and return the previous state; hand it back to restore. */
int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int state);

#endif
