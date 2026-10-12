#include "types.h"
#include "sdk/ctrdg.h"

BOOL CTRDG_IsAgbCartridge(void) {
  if (Cartridge_HeaderStillMatches() && Cartridge_HasGbaGame()) {
    return TRUE;
  }
  return FALSE;
}
