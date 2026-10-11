#include "sdk/os.h"

/* Set to 1 by Reset_OnOsTagMessage when a message with tag 0x10 arrives. */
extern volatile u16 data_022a9c34;

void Reset_ReloadProgramsFromCard(void);
void Reset_ClearAndJumpToEntry(void);

/* Waits for the reset request, then reloads both binaries and restarts. */
void OS_ResetSystem(void)
{
    while (data_022a9c34 == 0) {
    }
    *REG_IME = 0;
    Reset_ReloadProgramsFromCard();
    Reset_ClearAndJumpToEntry();
}
