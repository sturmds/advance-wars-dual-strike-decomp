#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;

int func_02124534(const char *a, const char *b);
char *strcpy(char *dst, const char *src);
u32 strlen(const char *str);
int Text_DrawStringToTiles(const char *str, u8 *tiles, int colour);

/* Returns the cache entry of str drawn in colour, drawing it first if the
 * cache does not have it. The cache keeps the strings from the start of
 * the font state, each ending in 0, with 1 after the last one; entry n
 * belongs to string n. A new string is drawn into the BG tiles from
 * digitEndTile on, which then moves past them. Returns -1 for an empty
 * string. The callees look like the C library's strcmp, strcpy and strlen. */
int FontCache_Add(const char *str, int colour)
{
    FontState *font = data_027c0280->fontState;
    char *names;
    int pos;
    int index;
    int tiles;

    if (str[0] == 0) {
        return -1;
    }
    names = (char *)font;
    pos = 0;
    index = 0;
    while (names[pos] != 1) {
        if (colour == font->cache[index].colour && func_02124534(names + pos, str) == 0) {
            return index;
        }
        pos = pos + strlen(names + pos) + 1;
        index++;
    }
    strcpy(names + pos, str);
    tiles = Text_DrawStringToTiles(str,
                          data_027c0280->bgVram + data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase * 0x4000
                              + font->digitEndTile * 32,
                          colour);
    font->cache[index].tile = font->digitEndTile;
    font->cache[index].halfTiles = tiles >> 1;
    font->cache[index].colour = colour;
    font->digitEndTile += tiles;
    names[pos + strlen(str) + 1] = 1;
    return index;
}
