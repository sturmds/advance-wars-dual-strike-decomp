#include "types.h"

typedef struct Unk_0208bf40_0 {
    u8 pad_00[0x18];
    u8 unk_18;
    u8 unk_19;
    u8 pad_1a[0x7d - 0x1a];
    u8 unk_7d;
    u8 pad_7e[0x84 - 0x7e];
    u8 unk_84;
} Unk_0208bf40_0;

extern Unk_0208bf40_0 *data_02160ac0;
extern u8 data_0229e148[];

void func_0208adac(void);
void func_02002e5c(const void *src, void *dst, u32 size, u32 unitBits);

/* Copies the 24 bytes at data_0229e148 into the block data_02160ac0 points to:
 * bytes 0-19 as a block, bytes 20-23 to offsets 0x18, 0x19, 0x7d and 0x84.
 * func_0208bfb0 reads them back. What the block holds is not known. */
void func_0208bf40(void)
{
    func_0208adac();
    func_02002e5c(data_0229e148, data_02160ac0, 20, 8);
    data_02160ac0->unk_18 = data_0229e148[20];
    data_02160ac0->unk_19 = data_0229e148[21];
    data_02160ac0->unk_7d = data_0229e148[22];
    data_02160ac0->unk_84 = data_0229e148[23];
}
