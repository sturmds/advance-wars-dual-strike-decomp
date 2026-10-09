#include "types.h"
#include "game/sound.h"

BOOL func_02108fc4(void **handle, int strmNo, u32 offset);
void func_02108fac(void **handle);

/* NitroSystem NNS_SndArcStrmStart: prepares stream strmNo on handle,
 * beginning at offset, then lets it play. Returns TRUE if it started. */
BOOL NNS_SndArcStrmStart(void **handle, int strmNo, u32 offset)
{
    if (!func_02108fc4(handle, strmNo, offset)) {
        return FALSE;
    }
    func_02108fac(handle);
    return TRUE;
}
