#include "types.h"
#include "game/Unk_02290718.h"

/* Picture variant stored in the saved record of a CO (0 or 1). */
int Co_GetSavedPictureVariant(int coId)
{
    if (coId >= CO_COUNT) {
        return 0;
    }
    return data_02290718.coRecords[coId].pictureVariant;
}
