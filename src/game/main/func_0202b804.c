#include "types.h"

void File_ReadAt(int, void*, int, int);
extern u8 data_02159928[];

void func_0202b804(int a0) { File_ReadAt(a0, data_02159928, 160, 0x3000); }
