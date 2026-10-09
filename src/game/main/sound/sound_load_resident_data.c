#include "types.h"

/* Sound archive loaders: each loads one item into the sound heap and
 * returns TRUE on success. */
BOOL func_02107804(int itemNo, void *heap);
BOOL func_021077cc(int itemNo, void *heap);

extern void *data_0218feac;                 /* sound heap */

/* Loads the sound archive items the game keeps resident from boot:
 * item 1 through func_02107804 and item 0x30 through func_021077cc. */
void Sound_LoadResidentData(void)
{
    func_02107804(1, data_0218feac);
    func_021077cc(0x30, data_0218feac);
}
