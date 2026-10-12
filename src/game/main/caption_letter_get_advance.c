#include "types.h"

int Text_GetCharWidth(int ch);

extern char gCaptionLetters[];

/* Returns the width of a caption letter, or 0 when the letter is not in
 * gCaptionLetters ("CONGRATULIS"). Callers pass the sprite position and tile
 * arguments too; this version does not use them. */
int CaptionLetter_GetAdvance(int ch, u16 x, u16 y, int attr, u16 firstTile,
                             u8 list) {
  int i;

  for (i = 0; gCaptionLetters[i] != '\0'; i++) {
    if (gCaptionLetters[i] == ch) {
      return Text_GetCharWidth(ch);
    }
  }
  return 0;
}
