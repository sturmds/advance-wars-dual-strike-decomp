#include "sdk/rtc.h"

/* The serial line routines, bit by bit: select the chip, send the command
 * byte, move the data bytes, release the chip. */
void func_arm7_027d6344(void);
void func_arm7_027d63b8(int mode, int reg);
void func_arm7_027d6428(void *data, int size);
void func_arm7_027d64b8(void *data, int size);
void func_arm7_027d6384(void);

void func_arm7_027d5b18(int mode, int reg, void *data, int size)
{
    func_arm7_027d6344();
    func_arm7_027d63b8(mode, reg);
    switch (mode) {
    case CLOCK_XFER_FROM_CHIP:
        func_arm7_027d64b8(data, size);
        break;
    case CLOCK_XFER_TO_CHIP:
        func_arm7_027d6428(data, size);
        break;
    }
    func_arm7_027d6384();
}
