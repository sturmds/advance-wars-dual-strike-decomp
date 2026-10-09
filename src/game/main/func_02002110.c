#include "types.h"
#include "game/co_graphics.h"

int Co_GetPictureVariant(int coId);
void func_02003a60(const void *src, void *dst, u32 size);
void Co_LoadPalette(int coId, int paletteSlot);
extern const void *const data_02152b78[][sizeof(CoGraphics) / sizeof(void *)];

void func_02002110(int coId, void *dest, int paletteSlot)
{
    int variant = Co_GetPictureVariant(coId);

    func_02003a60(data_02152b78[coId & 0x7f][variant], dest, 0x180);
    Co_LoadPalette(coId, paletteSlot);
}
