#include "types.h"

/* Walks a text string past its control codes and their arguments. Returns
 * FALSE at the first code 20, TRUE at the end of the string. Meaning of the
 * codes not known; the repeated 22/23 tests and the + 0xef are needed to match. */
BOOL func_02015dbc(const u8* text) {
  while (*text != 0) {
    switch (*text) {
      case 20:
        return FALSE;
      case 22:
      case 23:
        if (*text == 22 || *text == 22 || *text == 23 || *text == 23) {
          text++;
        }
        break;
      case 12:
      case 13:
      case 14:
      case 15:
      case 16:
      case 17:
      case 18:
      case 19:
      case 21:
        text++;
        break;
      case 10:
        text++;
        if ((u8)(*text + 0xef) <= 1) {
          text++;
        }
        break;
      case 9:
        text++;
        if (*text >= 0x80 && *text <= 0x99) {
          text++;
        } else if (*text >= 0x9a && *text <= 0xb3) {
          text++;
        }
        break;
      default:
        text += 2;
        break;
    }
  }
  return TRUE;
}
