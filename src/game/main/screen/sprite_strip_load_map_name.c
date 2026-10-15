#include "types.h"
#include "game/big_font.h"
#include "game/menu_state.h"
#include "game/Unk_02088d04.h"

const void* Map_GetName(u32 mapId);
u8* File_Load(const char* path);
void Data_Unpack(const void* src, void* dest);
void Mem_Free(void* block);

extern const BigFontGlyph data_0215bce0[];
extern const char data_0215bf38[];

/* Bytes per 4bpp tile. Needed to match: unsigned, or the compiler keeps
 * tile * 32 in its own register. */
#define TILE_BYTES 32u

/* SpriteStrip_Queue reads the x positions as signed. */
#define XS ((s16*)gMenuState.colours)

/* Loads the name of map mapId in the large sprite font (data_0215bf38's
 * file) into OBJ tiles from `tile` on, 16 tiles per letter, and sets up the
 * sprite strip for SpriteStrip_Queue: one x position per letter, the line
 * centred on the 256-pixel screen. A character the font lacks uses the
 * table's last glyph. */
void SpriteStrip_LoadMapName(int mapId, int tile) {
  u8 c;
  const BigFontGlyph* glyph;
  int glyphWidth;
  const u8* name;
  int width;
  int count;
  u8* file;

  name = Map_GetName((u16)mapId);
  width = 0;
  count = 0;
  file = File_Load(data_0215bf38);
  for (; *name != 0; name++) {
    XS[count] = width;
    c = *name;
    glyph = data_0215bce0;
    while (TRUE) {
      if (glyph->character == 0 || glyph->character == c) {
        Data_Unpack(file + glyph->offset,
                    data_027c0280->objVram + tile * TILE_BYTES);
        glyphWidth = glyph->width;
        tile += 16;
        XS[count] -= (32 - glyphWidth) >> 1;
        width += glyphWidth;
        count++;
        break;
      }
      glyph++;
    }
  }
  gMenuState.spriteStrip.spriteCount = count;
  /* Needed to match: count doubles as the loop counter. */
  for (count = 0; count < gMenuState.spriteStrip.spriteCount; count++) {
    XS[count] += (256 - width) >> 1;
  }
  Mem_Free(file);
}
