#include "types.h"
#include "game/Unk_02088d04.h"
#include "sdk/msl.h"

int FontCache_Add(const char* str, int colour);
void Text_PutGlyphTiles(u16* map, int tile, int count);

/* Draws str in colour through the font cache and writes its tiles into the
 * screen map at map, with tileBase added to the cache's tile numbers.
 * Returns the length of str, or 0 when the cache could not take it. */
u32 Text_WriteCachedString(u16* map, const char* str, int tileBase,
                           int colour) {
  FontState* font = data_027c0280->fontState;
  int index;
  u16 tile;

  index = FontCache_Add(str, colour);
  if (index == -1) {
    return 0;
  }
  tile = tileBase + font->cache[index].tile;
  Text_PutGlyphTiles(map, tile, font->cache[index].halfTiles);
  return strlen(str);
}
