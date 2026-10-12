#include "types.h"
#include "game/icon.h"

/* Byte offset of icon iconId in the file of its bank: the icons of a bank
 * are stored one after another, starting with the bank's first icon.
 * Returns 0 if iconId comes before the bank's first icon. */
u32 Icon_GetFileOffset(int iconId, int bank) {
  int i;
  u32 tiles;

  tiles = 0;
  i = gIconBankFirstIcon[bank * 4];
  do {
    if (i == iconId) {
      goto found; /* needed to match: the found case sits after the loop */
    }
    tiles += gIconSize[i][0] * gIconSize[i][1];
    i++;
  } while (i < ICON_COUNT);
  return 0;

found:
  return tiles * 32;
}
