#include "types.h"
#include "sdk/os.h"

extern u32 data_022a9b78;
u32 func_0210dbec(void);
u32 func_0210e36c(int id);
void func_0210e270(int id, u32 lo);
void OS_SetArenaLo(int id, u32 hi);
void func_0210e67c(u32 value);
u32 OS_GetInitArenaHi(int id);

/* Sets the low and high marks of arena 2; then, when data_022a9b78 is 0 or the console type reads 1, sets protection region 1 to 4 MB at 0x02000000. */
void func_0210e4bc(void)
{
    func_0210e270(2, func_0210e36c(2));
    OS_SetArenaLo(2, OS_GetInitArenaHi(2));
    if (data_022a9b78 == 0 || (func_0210dbec() & 3) == 1) {
        func_0210e67c(0x0200002b);
    }
}
