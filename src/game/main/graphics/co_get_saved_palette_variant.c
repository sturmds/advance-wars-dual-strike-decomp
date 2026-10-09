#include "types.h"
#include "game/Unk_02290718.h"

/* Palette variant (0-7) stored in the saved record of a CO; 0 for ids past the last CO. */
int Co_GetSavedPaletteVariant(int coId)
{
    if (coId >= CO_COUNT) {
        return 0;
    }
    return data_02290718.coRecords[coId].paletteVariant;
}
