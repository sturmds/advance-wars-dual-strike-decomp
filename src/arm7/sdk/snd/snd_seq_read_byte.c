#include "sdk/snd_seq.h"

extern u8 data_arm7_0380807c[16]; /* the bytes of data_arm7_03808074 */

void func_arm7_038013f4(u8 *address);

u8 SndSeq_ReadByte(u8 *address)
{
    if (address < data_arm7_03808074.start || address >= data_arm7_03808074.end) {
        func_arm7_038013f4(address);
    }
    return data_arm7_0380807c[address - data_arm7_03808074.start];
}
