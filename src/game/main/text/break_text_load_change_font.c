#include "types.h"
#include "game/break_text.h"
#include "game/Unk_02088d04.h"

void* Mem_Alloc(u32 size);
void Mem_Free(void* res);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Palette_Load(const void* src, int offset, u32 size);
void Data_Unpack(const void* src, void* dest);
u8* File_Load(const char* path);

extern Unk_027c0280* data_027c0280;
extern char data_021597c8[]; /* "/ohashi/res_changefont" */

#define CHANGE_FONT_GLYPHS 27         /* A-Z, then '*' */
#define CHANGE_FONT_GLYPH_BYTES 0x100 /* 8 OBJ tiles: 16x32 pixels */

/* Like BreakText_Load, with the font res_changefont: sets up *gBreakText
 * for text, which may hold only 'A'-'Z' and '*', each 16 pixels wide. Loads
 * the glyphs into OBJ VRAM from tile on and the font palette into OBJ
 * palette number palette. Returns the number of letters. */
u8 BreakText_LoadChangeFont(const s8* text, u16 tile, u8 palette) {
  int count;
  int x;
  u16 glyphs[32];
  int i;
  u8* font;
  u8* res;

  count = 0;
  x = count;
  while (text[count] != 0) {
    gBreakText->letterX[count] = x;
    x += 16;
    /* needed to match: an if/else becomes conditional stores */
    switch (text[count]) {
      case '*':
        glyphs[count] = 26;
        break;
      default:
        glyphs[count] = text[count] - 'A';
        break;
    }
    count++;
  }
  gBreakText->width = x;
  gBreakText->tile = tile;
  gBreakText->palette = palette;
  font = Mem_Alloc(CHANGE_FONT_GLYPHS * CHANGE_FONT_GLYPH_BYTES);
  res = File_Load(data_021597c8);
  Data_Unpack(res, font);
  for (i = 0; i < count; i++) {
    Mem_Copy32(font + glyphs[i] * CHANGE_FONT_GLYPH_BYTES,
               data_027c0280->objVram + (gBreakText->tile + i * 8) * 32,
               CHANGE_FONT_GLYPH_BYTES);
  }
  Palette_Load(res + 0x428, (u16)((gBreakText->palette + 16) << 5), 32);
  Mem_Free(res);
  Mem_Free(font);
  return count;
}
