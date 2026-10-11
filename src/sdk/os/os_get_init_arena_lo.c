#include "types.h"
#include "sdk/os.h"

extern u32 data_022a9b78;
u32 func_0210dbec(void);

/* Initial low end of each arena; 0 for an id with no arena. */
u32 OS_GetInitArenaLo(int id)
{
    switch (id) {
    case 0:
        return 0x0237a5a0;
    case 2:
        if (data_022a9b78 == 0 || (func_0210dbec() & 3) == 1) {
            return 0;
        }
        return 0x02400000;
    case 3:
        return 0x01ffde80;
    case 4:
        return 0x027c12e0;
    case 5:
        return 0x027ff000;
    case 6:
        return 0x037f8000;
    default:
        return 0;
    }
}
