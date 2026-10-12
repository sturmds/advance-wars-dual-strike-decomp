#include "types.h"
#include "game/sound.h"

BOOL SoundArc_PrepareStream(void** handle, int strmNo, u32 offset);
void func_02108fac(void** handle);

/* NitroSystem NNS_SndArcStrmStart: prepares stream strmNo on handle,
 * beginning at offset, then lets it play. Returns TRUE if it started. */
BOOL NNS_SndArcStrmStart(void** handle, int strmNo, u32 offset) {
  if (!SoundArc_PrepareStream(handle, strmNo, offset)) {
    return FALSE;
  }
  func_02108fac(handle);
  return TRUE;
}
