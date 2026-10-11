#include "types.h"

extern int data_022a7e68;
void DownloadPlay_StopParent(void *, int);

void func_020fc7d8(void)
{
    data_022a7e68 = 4;
    DownloadPlay_StopParent(&data_022a7e68, 4);
}
