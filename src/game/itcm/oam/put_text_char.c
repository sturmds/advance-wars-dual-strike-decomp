#include "types.h"

/* Needed to match: declared with u16 parameters, so y - 7 and the computed
 * tiles are narrowed here (oam/put_text_tile.c defines it with int). */
void Oam_PutTextTile(u16 x, u16 y, u16 tile);

/* Draws one character of sprite text at (x, y) and returns whether the
 * cursor moves on. The marks ~ ' ^ are drawn 7 pixels higher and return
 * FALSE, so the next character is drawn under them. */
BOOL Oam_PutTextChar(u16 x, u16 y, u8 ch) {
  switch (ch) {
    case ',':
    case '.':
      Oam_PutTextTile(x, y, 0x39c);
      return TRUE;
    case '-':
      Oam_PutTextTile(x, y, 0x39f);
      return TRUE;
    case '=':
      Oam_PutTextTile(x, y, 0x39d);
      return TRUE;
    case '_':
      Oam_PutTextTile(x, y, 0x39d);
      return TRUE;
    case '(':
    case '<':
      Oam_PutTextTile(x, y, 0x39a);
      return TRUE;
    case ')':
    case '>':
      Oam_PutTextTile(x, y, 0x39b);
      return TRUE;
    case ':':
      Oam_PutTextTile(x, y, 0x39d);
      return TRUE;
    case ' ':
      return TRUE;
    case '/':
      Oam_PutTextTile(x, y, 0x39d);
      return TRUE;
    case '%':
      Oam_PutTextTile(x, y, 0x39e);
      return TRUE;
    case '?':
      Oam_PutTextTile(x, y, 0x37c);
      return TRUE;
    case '~':
      Oam_PutTextTile(x, y - 7, 0x37d);
      return FALSE;
    case '\'':
      Oam_PutTextTile(x, y - 7, 0x37e);
      return FALSE;
    case '^':
      Oam_PutTextTile(x, y - 7, 0x37f);
      return FALSE;
  }
  /* Lower case uses the upper case glyphs. */
  if (ch <= 'z' && ch >= 'a') {
    Oam_PutTextTile(x, y, ch + 0x31f);
  } else if (ch <= 'Z' && ch >= 'A') {
    Oam_PutTextTile(x, y, ch + 0x33f);
  } else {
    Oam_PutTextTile(x, y, ch + 0x340);
  }
  return TRUE;
}
