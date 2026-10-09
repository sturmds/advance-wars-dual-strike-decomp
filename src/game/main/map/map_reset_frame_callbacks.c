#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void func_02004510(void (*callback)(void));
void func_01ff9c28(void (*callback)(void));
void func_01ff9c48(void (*callback)(void));
void func_020c0704(void);
void func_020d1be0(void);
void func_020d1c3c(void);
void func_02000d24(void);
void func_02000fa4(void);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0288 data_027c0288;

/* Name is a guess. Drops the per-frame callback of the current map screen
 * (func_020d1c3c for the main map, func_020d1be0 for the second) and puts
 * the main loop's two callbacks back to func_02000d24 and func_02000fa4. */
void Map_ResetFrameCallbacks(void)
{
    if ((void *)data_027c027c == data_027c0288.unk_00) {
        func_02004510(func_020d1c3c);
    } else {
        func_02004510(func_020d1be0);
    }
    func_01ff9c28(func_02000d24);
    func_01ff9c48(func_02000fa4);
    func_020c0704();
}
