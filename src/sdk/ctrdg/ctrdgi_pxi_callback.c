#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os.h"

int func_0211e628(void);

extern int (*gCtrdgPulledOutHook)(void);

void Cartridge_OnRemovalMessage(u32 tag, u32 data, int err)
{
    int result;

    if ((data & GBA_SLOT_MSG_OPCODE_BITS) == GBA_SLOT_MSG_REMOVED) {
        result = 0;
        if (gCtrdgPulledOutHook != NULL) {
            result = gCtrdgPulledOutHook();
        }
        if (result != 0) {
            func_0211e628();
        }
    } else {
        OS_Terminate();
    }
}
