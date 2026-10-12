#include "types.h"

int CaptionFont_CharWidth(int ch);

int CaptionFont_TextWidth(const u8* str) {
  int width = 0;

  while (*str != 0) {
    width += CaptionFont_CharWidth(*str);
    str++;
  }
  return width;
}
