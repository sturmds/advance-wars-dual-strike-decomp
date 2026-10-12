#include "types.h"

/* Glyph index of a name character: A-Z are 0-25, then , . - and an apostrophe;
 * a space or any other character gives 31. */
int Ov005_CharToGlyphIndex(u32 ch) {
  if (ch >= 'A' && ch <= 'Z') {
    return ch - 'A';
  }
  if (ch == ',') {
    return 26;
  }
  if (ch == '.') {
    return 27;
  }
  if (ch == '-') {
    return 28;
  }
  if (ch == ' ') {
    return 31;
  }
  if (ch == '\'') {
    return 29;
  }
  return 31;
}
