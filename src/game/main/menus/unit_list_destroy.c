#include "types.h"

void Mem_Free(void *ptr);
void func_01ff8c84(int layer);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
void Font_LoadDigits(int a);

extern u8 *gUnitListRows;

/* Frees the unit list rows and clears both screens' layers and the digit font. */
void UnitList_Destroy(void)
{
    Mem_Free(gUnitListRows);
    func_01ff8c84(0);
    func_01ff8c84(1);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    func_020c4018();
    Font_LoadDigits(0);
}
