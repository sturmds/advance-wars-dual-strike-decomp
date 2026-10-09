#include "types.h"

void *ResultObj_Load(int firstTile, int palette);
void Mem_Free(void *res);

void func_02039f80(int firstTile, int palette)
{
    Mem_Free(ResultObj_Load(firstTile, palette));
}
