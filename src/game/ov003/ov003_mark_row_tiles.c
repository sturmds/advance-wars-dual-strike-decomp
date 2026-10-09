#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02088d04.h"

#define MAP_TILE_MAX 0x508

typedef struct Ov003Map {
    u8 width;
    u8 height;
    u16 cells[MAP_TILE_MAX];           /* tile code per tile, row by row */
    u8 marks[MAP_TILE_MAX];            /* per tile: MARK_* bits, set by this function */
} Ov003Map;

/* Ov003Map.marks bits. */
#define MARK_OWNED    1                /* the tile belongs to the selected unit's row */
#define MARK_NEXT_TO  2                /* a tile next to a marked one */
#define MARK_UNIT     4                /* a unit of the current row stands here */

typedef struct Ov003Slot {
    u8 pad_00;
    u8 row : 4;
    u8 pad_02[2];
} Ov003Slot;

typedef struct Ov003State {
    u8 pad_00[0x138];
    Ov003Slot slots[1];                /* length unknown */
    u8 pad_13c[0x1d5 - 0x13c];
    u8 currentSlot;
    u8 pad_1d6[0x1df - 0x1d6];
    u8 unk_1df_0 : 1;
} Ov003State;

extern Ov003Map *data_ov003_02364a88;
extern Ov003State *data_ov003_02364928;
extern Ov003Unit *data_ov003_02369534;
extern Ov003Unit data_ov003_02369538[16];
extern u8 data_ov003_02365d10[];       /* per tile code: bits 5-7 row, bits 0-4 terrain */
extern Unk_027c0280 *data_027c0280;

int func_ov003_023622b8(void);
int func_ov003_02361f38(void);
void func_ov003_02356614(void);
void func_02002d9c(u32 value, void *dst, u32 size);
void Mem_Zero32(u32 value, void *dst, u32 size);

#define MAP data_ov003_02364a88
#define STATE data_ov003_02364928
#define TILE_AT(x, y) ((y) * MAP->width + (x))

/* Fills Ov003Map.marks for drawing. When func_ov003_02361f38() is 1 or
 * Ov003State.unk_1df_0 is clear, every mark is set to 0xff and only BG0, BG3
 * and the sprites are shown. Otherwise it marks the tiles of the row of
 * data_ov003_02369534 (except terrain 21 and 26) and the tiles next to them,
 * then the tiles of the current slot's units and the tiles next to those, and
 * shows all layers. */
void Ov003_MarkRowTiles(void)
{
    Ov003Unit *unit;
    int x;
    int y;
    int i;

    unit = data_ov003_02369538;
    if (func_ov003_02361f38() == 1 || !STATE->unk_1df_0) {
        func_02002d9c(-1, MAP->marks, MAP_TILE_MAX);
        GFX_DISPCNT(data_027c0280)->bg0 = 1;
        GFX_DISPCNT(data_027c0280)->bg1 = 0;
        GFX_DISPCNT(data_027c0280)->bg2 = 0;
        GFX_DISPCNT(data_027c0280)->bg3 = 1;
        GFX_DISPCNT(data_027c0280)->obj = 1;
        return;
    }
    Mem_Zero32(0, MAP->marks, MAP_TILE_MAX);
    for (y = 0; y < MAP->height; y++) {
        for (x = 0; x < MAP->width; x++) {
            u8 tile = data_ov003_02365d10[MAP->cells[TILE_AT(x, y)]];

            if (data_ov003_02369534->unk_12_3 != ((tile >> 5) & 7) || (tile & 31) == 21 || (tile & 31) == 26) {
                continue;
            }
            MAP->marks[TILE_AT(x, y)] |= MARK_OWNED;
            if (x > 0) {
                MAP->marks[TILE_AT(x - 1, y)] |= MARK_NEXT_TO;
            }
            if (y > 0) {
                MAP->marks[TILE_AT(x, y - 1)] |= MARK_NEXT_TO;
            }
            if (x < MAP->width - 1) {
                MAP->marks[TILE_AT(x + 1, y)] |= MARK_NEXT_TO;
            }
            if (y < MAP->height - 1) {
                MAP->marks[TILE_AT(x, y + 1)] |= MARK_NEXT_TO;
            }
        }
    }
    for (i = 0; i < func_ov003_023622b8(); i++, unit++) {
        if (unit->hp == 0 && !unit->unk_17_7) {
            continue;
        }
        if (unit->unk_12_3 != STATE->slots[STATE->currentSlot].row) {
            continue;
        }
        x = unit->x >> 7;
        y = unit->y >> 7;
        MAP->marks[TILE_AT(x, y)] |= MARK_UNIT;
        if (x > 0) {
            MAP->marks[TILE_AT(x - 1, y)] |= MARK_NEXT_TO;
        }
        if (y > 0) {
            MAP->marks[TILE_AT(x, y - 1)] |= MARK_NEXT_TO;
        }
        if (x < MAP->width - 1) {
            MAP->marks[TILE_AT(x + 1, y)] |= MARK_NEXT_TO;
        }
        if (y < MAP->height - 1) {
            MAP->marks[TILE_AT(x, y + 1)] |= MARK_NEXT_TO;
        }
    }
    func_ov003_02356614();
    GFX_DISPCNT(data_027c0280)->bg0 = 1;
    GFX_DISPCNT(data_027c0280)->bg1 = 1;
    GFX_DISPCNT(data_027c0280)->bg2 = 1;
    GFX_DISPCNT(data_027c0280)->bg3 = 1;
    GFX_DISPCNT(data_027c0280)->obj = 1;
}
