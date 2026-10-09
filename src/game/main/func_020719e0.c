#include "types.h"
#include "game/Unk_02088d04.h"

/* Two parallel tables, 44 rows of 44 entries of 0x30 bytes: a text id, and a
 * function that returns the number to show next to it. */
typedef struct Unk_0215d6c4 {
    u32 textId;
    u8 pad_04[0x30 - 0x04];
} Unk_0215d6c4;

typedef struct Unk_0215d6d8 {
    int (*getValue)(int, int);
    u8 pad_04[0x30 - 0x04];
} Unk_0215d6d8;

void Bg_PutTileQuad(int x, int y, int firstTile, int palette);
void func_020153d4(s16 x, s16 y, u16 *map, u32 textId, u16 a, u16 b);
void func_02014d14(s16 x, s16 y, u16 *map, int value, u16 a, u16 b);

extern Unk_027c0280 *data_027c0280;
extern Unk_0215d6c4 data_0215d6c4[][44];
extern Unk_0215d6d8 data_0215d6d8[][44];

/* Draws one table entry on layer 0 at (x, y): a tile quad, the entry's text
 * at x + 2 and its number at x + 20. */
void func_020719e0(int x, int y, int row, int col)
{
    u16 *map;

    Bg_PutTileQuad(x, y, 0x3fc, 2);
    func_020153d4(x + 2, y, data_027c0280->layer0Map, data_0215d6c4[row][col].textId, 0, ((y - 5) >> 1) * 64 + 128);
    /* Needed to match: the map is read into a local before the call. */
    map = data_027c0280->layer0Map;
    func_02014d14(x + 20, y, map, data_0215d6d8[row][col].getValue(row, col), 0, 0);
}
