#include "types.h"
#include "game/break_text.h"
#include "game/Unk_02088d04.h"

void Palette_Load(const void* src, int offset, u32 size);
void Data_Unpack(const void* src, void* dest);
u8* File_Load(const char* path);
void Mem_Free(void* res);

extern s8 data_02159684[];   /* letters of the font, in glyph order */
extern char data_021597e0[]; /* "/ohashi/res_tagbreakfont" */
extern char data_021597fc[]; /* "/ohashi/res_breakfont" */
extern u16 data_02159748[];  /* per glyph: offset in res_tagbreakfont */
extern u16 data_021596c8[];  /* per glyph: offset in res_breakfont */

#define NO_GLYPH 0xffff

/* OBJ tiles per letter: 32x32 pixels in the tag font, 16x32 otherwise. */
#define LETTER_TILES ((gBreakText->tagFont == 1 ? 4 : 2) * 4)

/* Sets up *gBreakText for text: finds each letter in the font, works
 * out its x, then loads the glyphs into OBJ VRAM from tile on and the font
 * palette into OBJ palette number palette. Letters missing from the font
 * are not drawn and take no width. Returns the number of letters. */
u8 BreakText_Load(const s8* text, u16 tile, u8 palette, u32 tagFont) {
  int count;
  u8* tagFontRes;
  int glyph;
  int x;
  u16 glyphs[32];
  /* needed to match: each branch below has its own counter and file */
  int letter;
  int tagLetter;
  u8* fontRes;

  count = 0;
  glyph = count;
  x = count;
  gBreakText->tagFont = tagFont;
  while (text[count] != 0) {
    gBreakText->letterX[count] = x;
    if (data_02159684[glyph] == text[count]) {
      switch (text[count]) {
        case ' ':
          x += 4;
          break;
        case '-':
          x += 10;
          break;
        case -19: /* 0xed */
        case '\'':
        case 'i':
        case 'l':
          x += 8;
          break;
        case -17: /* 0xef */
        case 'I':
        case 'j':
          x += 12;
          break;
        default:
          x += 15;
          if (tagFont == 1 && text[count] == 'W') {
            x += 4;
          }
          break;
      }
      if (tagFont == 1) {
        x += 2;
      }
      /* centre the letter in its 32 or 16 pixel cell */
      if (tagFont == 1) {
        gBreakText->letterX[count] -=
            (32 - (x - gBreakText->letterX[count])) >> 1;
      } else {
        gBreakText->letterX[count] -=
            (16 - (x - gBreakText->letterX[count])) >> 1;
      }
      glyphs[count] = glyph;
      glyph = 0;
      count++;
    } else {
      glyph++;
      if (data_02159684[glyph] == 0) {
        glyphs[count] = NO_GLYPH;
        glyph = 0;
        count++;
      }
    }
  }
  gBreakText->width = x;
  gBreakText->tile = tile;
  gBreakText->palette = palette;
  if (tagFont == 1) {
    tagFontRes = File_Load(data_021597e0);
    for (tagLetter = 0; tagLetter < count; tagLetter++) {
      if (glyphs[tagLetter] != NO_GLYPH) {
        Data_Unpack(tagFontRes + data_02159748[glyphs[tagLetter]],
                    data_027c0280->objVram +
                        (LETTER_TILES * tagLetter + gBreakText->tile) * 32);
      }
    }
    Palette_Load(tagFontRes + 0x2310, (u16)((gBreakText->palette + 16) << 5),
                 32);
    Mem_Free(tagFontRes);
  } else {
    fontRes = File_Load(data_021597fc);
    for (letter = 0; letter < count; letter++) {
      if (glyphs[letter] != NO_GLYPH) {
        Data_Unpack(fontRes + data_021596c8[glyphs[letter]],
                    data_027c0280->objVram +
                        (LETTER_TILES * letter + gBreakText->tile) * 32);
      }
    }
    Palette_Load(fontRes + 0x1624, (u16)((gBreakText->palette + 16) << 5), 32);
    Mem_Free(fontRes);
  }
  return count;
}
