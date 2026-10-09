#include "types.h"

void func_0210a948(void);
int File_UnpackTo(int, void *);
extern u8 data_02160aa0[];

int func_020828d0(void)
{
    func_0210a948();
    return File_UnpackTo(0x6800000, data_02160aa0);
}
