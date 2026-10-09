#include "types.h"

void func_020a756c(void);
void func_020a73e4(void);
BOOL func_020a870c(s16 *tile);
BOOL func_020a7034(s16 *tile);

/* Runs the two setup passes, then two tile searches in turn. Writes the
 * tile to out only when both succeed. */
BOOL func_020aac60(s16 *out)
{
    s16 tile[2];

    func_020a756c();
    func_020a73e4();
    if (!func_020a870c(tile)) {
        return FALSE;
    }
    if (!func_020a7034(tile)) {
        return FALSE;
    }
    out[0] = tile[0];
    out[1] = tile[1];
    return TRUE;
}
