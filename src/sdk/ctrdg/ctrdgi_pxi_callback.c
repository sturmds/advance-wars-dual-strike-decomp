#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os.h"

int func_0211e628(void);

extern int (*data_022acf20)(void);

void Cartridge_OnRemovalMessage(u32 tag, u32 data, int err)
{
    int result;

    if ((data & GBA_SLOT_MSG_OPCODE_BITS) == GBA_SLOT_MSG_REMOVED) {
        result = 0;
        if (data_022acf20 != NULL) {
            result = data_022acf20();
        }
        if (result != 0) {
            func_0211e628();
        }
    } else {
        OS_Terminate();
    }
}
