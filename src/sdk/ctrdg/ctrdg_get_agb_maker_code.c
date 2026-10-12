#include "types.h"
#include "sdk/ctrdg.h"

u16 CTRDG_GetAgbMakerCode(void) {
  u16 makerCode = 0;

  if (Cartridge_HeaderStillMatches()) {
    makerCode = Cartridge_GbaMakerCodeOrZero();
  }
  return makerCode;
}
