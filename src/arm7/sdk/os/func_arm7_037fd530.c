#include "types.h"

extern s32 data_arm7_03807a9c;
extern s32 data_arm7_03807aa0;
int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/*
 * V-count alarm frame counter: a V-count below the one seen last means a
 * new frame started. Records vcount and returns the frame count.
 */
s32 func_arm7_037fd530(s32 vcount)
{
    int irqState = func_arm7_037fdb80();

    if (vcount < data_arm7_03807a9c) {
        data_arm7_03807aa0++;
    }
    data_arm7_03807a9c = vcount;
    func_arm7_037fdb94(irqState);
    return data_arm7_03807aa0;
}
