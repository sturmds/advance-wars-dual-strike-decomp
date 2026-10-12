#include "types.h"
#include "game/icon.h"

/* Bank of icon iconId. The bounds match gIconBankFirstIcon. */
int Icon_GetBank(int iconId) {
  if (iconId >= 276) {
    return 5;
  }
  if (iconId >= 102) {
    return 4;
  }
  if (iconId >= 90) {
    return 3;
  }
  if (iconId >= 89) {
    return 2;
  }
  if (iconId >= 75) {
    return 1;
  }
  return 0;
}
