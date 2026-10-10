#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov003_02364a84.h"
#include "game/Unk_ov003_02369bb8.h"
#include "game/terrain.h"

extern Unk_027c0280 *data_027c0280;
extern u8 data_ov003_02365d10[];       /* per tile code: bits 5-7 owner, bits 0-4 terrain */
extern u16 data_ov003_02366110[512][4]; /* layer 1 map entries per 16x16 tile */
extern u16 data_ov003_02367110[512][4]; /* layer 2 and 3 map entries per 16x16 tile */

Unk_ov003_02369bb8 *func_ov003_0235e8fc(int x, int y);
int func_ov003_02361f38(void);
void Ov003_DrawTexturedQuad(int x, int y, int depth, int arg3, int arg4, int tile, int palette, int arg7, int arg8);

#define MAP data_ov003_02364a88
#define STATE data_ov003_02364928
#define TILE_AT(x, y) ((y) * MAP->width + (x))
/* Scroll in pixels; read signed here. */
#define SCROLL_X ((s16)STATE->unk_1d0)
#define SCROLL_Y ((s16)STATE->unk_1d2)

/* Map entry index of the top left 8x8 cell of a 16x16 tile on screen. */
#define CELL(sx, sy) (((sx) + (sy) * 32) * 2)

/* Map entry k (0-3: top left, top right, bottom left, bottom right) of a
 * 16x16 tile in one of the tile tables. Needed to match: read as a flat u16
 * array, except in the HQ case. */
#define TILE_ENTRY(table, tile, k) (((u16 *)(table))[(tile) * 4 + (k)])

/* Writes a 16x16 tile into a layer's map with BG palette pal. */
#define PUT_TILE(layer, cell, table, tile, pal)                                            \
    do {                                                                                   \
        data_027c0280->layer[(cell)] = (TILE_ENTRY(table, tile, 0) & 0xfff) | ((pal) << 12);      \
        data_027c0280->layer[(cell) + 1] = (TILE_ENTRY(table, tile, 1) & 0xfff) | ((pal) << 12);  \
        data_027c0280->layer[(cell) + 32] = (TILE_ENTRY(table, tile, 2) & 0xfff) | ((pal) << 12); \
        data_027c0280->layer[(cell) + 33] = (TILE_ENTRY(table, tile, 3) & 0xfff) | ((pal) << 12); \
    } while (0)

/* Same as PUT_TILE, through the table's [tile][k] form. Needed to match: the
 * HQ case reads the table this way. */
#define PUT_TILE_HQ(layer, cell, table, tile, pal)                                      \
    do {                                                                                \
        data_027c0280->layer[(cell)] = ((table)[(tile)][0] & 0xfff) | ((pal) << 12);      \
        data_027c0280->layer[(cell) + 1] = ((table)[(tile)][1] & 0xfff) | ((pal) << 12);  \
        data_027c0280->layer[(cell) + 32] = ((table)[(tile)][2] & 0xfff) | ((pal) << 12); \
        data_027c0280->layer[(cell) + 33] = ((table)[(tile)][3] & 0xfff) | ((pal) << 12); \
    } while (0)

/* Same as PUT_TILE, keeping the palette of the table entries. */
#define PUT_TILE_RAW(layer, cell, table, tile)                           \
    do {                                                                 \
        data_027c0280->layer[(cell)] = TILE_ENTRY(table, tile, 0);      \
        data_027c0280->layer[(cell) + 1] = TILE_ENTRY(table, tile, 1);  \
        data_027c0280->layer[(cell) + 32] = TILE_ENTRY(table, tile, 2); \
        data_027c0280->layer[(cell) + 33] = TILE_ENTRY(table, tile, 3); \
    } while (0)

/* Owner bits of a data_ov003_02365d10 entry: 0 neutral, 1-4 an army. */
#define OWNER(info) (((info) >> 5) & 7)

/* A 16x16 sprite over map tile (x, y). */
#define SPRITE(x, y, depth, tile, palette) \
    Ov003_DrawTexturedQuad((x) * 16 - SCROLL_X, (y) * 16 - SCROLL_Y, depth, 1, 1, tile, palette, 0, 0)

/* Draws map tile (x, y) when it is on screen: its lower half into layers 3
 * and 2, its upper half into layer 1 one row up, and the sprites that stand
 * on it (trees, reefs, building tops). Terrain ids 9 and 25 have no name in
 * terrain.h yet. For a Minicannon the owner bits pick one of four sprites. */
void Ov003_DrawTile(u8 x, u8 y)
{
    Unk_ov003_02369bb8 *object;
    u16 code;
    u8 mark;
    int info;
    int terrain;
    u8 sx;
    u8 sy;
    int tile;
    BOOL drawn;
    u8 below;
    int owner;

    object = NULL;
    code = MAP->cells[TILE_AT(x, y)];
    mark = MAP->marks[TILE_AT(x, y)];
    info = data_ov003_02365d10[code];
    terrain = info & 31;
    if (terrain == 9 || terrain == TERRAIN_MINICANNON || terrain == TERRAIN_BLACK_CANNON) {
        object = func_ov003_0235e8fc(x, y);
    }

    sx = x - (SCROLL_X >> 4);
    sy = y - (SCROLL_Y >> 4);
    if (sx <= 16 && sy <= 12) {
        /* the 17th column goes to the second 32x32 screen block */
        if (sx >= 16) {
            sx -= 16;
            sy += 16;
        }
        if (terrain == 9 || terrain == TERRAIN_MINICANNON || terrain == TERRAIN_BLACK_CANNON) {
            if (object->unk_09_6) {
                PUT_TILE(layer3Map, CELL(sx, sy), data_ov003_02367110, code, 13);
            } else {
                PUT_TILE_RAW(layer3Map, CELL(sx, sy), data_ov003_02367110, code);
            }
        } else {
            PUT_TILE_RAW(layer3Map, CELL(sx, sy), data_ov003_02367110, code);
        }

        switch (terrain) {
        case 9:
        case TERRAIN_MINICANNON:
        case 25:
        case TERRAIN_BLACK_CANNON:
            PUT_TILE(layer2Map, CELL(sx, sy), data_ov003_02367110, code, 12);
            break;
        case TERRAIN_CITY:
            PUT_TILE(layer2Map, CELL(sx, sy), data_ov003_02367110, 450, 7);
            break;
        case TERRAIN_BASE:
            PUT_TILE(layer2Map, CELL(sx, sy), data_ov003_02367110, 449, 7);
            break;
        case TERRAIN_AIRPORT:
            PUT_TILE(layer2Map, CELL(sx, sy), data_ov003_02367110, 451, 7);
            break;
        case TERRAIN_PORT:
            PUT_TILE(layer2Map, CELL(sx, sy), data_ov003_02367110, 452, 7);
            break;
        case TERRAIN_COM_TOWER:
            PUT_TILE(layer2Map, CELL(sx, sy), data_ov003_02367110, 441, 7);
            break;
        case TERRAIN_LAB:
            PUT_TILE(layer2Map, CELL(sx, sy), data_ov003_02367110, 473, 7);
            break;
        case TERRAIN_HQ:
            switch (OWNER(info)) {
            case 1:
                tile = 485;
                break;
            case 2:
                tile = 490;
                break;
            case 3:
                tile = 495;
                break;
            case 4:
                tile = 500;
                break;
            default:
                tile = code;
                break;
            }
            PUT_TILE_HQ(layer2Map, CELL(sx, sy), data_ov003_02367110, tile, 7);
            break;
        default:
            PUT_TILE(layer2Map, CELL(sx, sy), data_ov003_02367110, code, 7);
            break;
        }

        if (func_ov003_02361f38() == 0 && STATE->unk_1df_0 && mark == 0) {
            /* When a tree or reef sprite went here, the top of the tile
             * below is drawn here as a sprite too, at depth 40. */
            drawn = FALSE;
            switch (terrain) {
            case TERRAIN_WOOD:
                SPRITE(x, y, 30, 72, 2);
                drawn = TRUE;
                break;
            case TERRAIN_REEF:
                SPRITE(x, y, 30, data_ov003_02364a84->unk_09_2 * 4 + 76, 2);
                drawn = TRUE;
                break;
            }
            if (drawn && y < MAP->height - 1) {
                below = data_ov003_02365d10[MAP->cells[TILE_AT(x, y + 1)]];
                switch (below & 31) {
                case TERRAIN_MOUNTAIN:
                    SPRITE(x, y, 40, 0, 0);
                    break;
                case TERRAIN_WOOD:
                    SPRITE(x, y, 40, 4, 1);
                    break;
                case TERRAIN_HQ:
                    owner = OWNER(below);
                    SPRITE(x, y, 40, (owner - 1) * 4 + 8, owner + 3);
                    break;
                case TERRAIN_CITY:
                    SPRITE(x, y, 40, 28, OWNER(below) + 3);
                    break;
                case TERRAIN_BASE:
                    if ((owner = OWNER(below)) != 0) {
                        SPRITE(x, y, 40, data_ov003_02364a84->unk_0b_2 * 4 + 56, owner + 3);
                    }
                    break;
                case TERRAIN_AIRPORT:
                    SPRITE(x, y, 40, 36, OWNER(below) + 3);
                    break;
                case TERRAIN_PORT:
                    SPRITE(x, y, 40, 40, OWNER(below) + 3);
                    break;
                case TERRAIN_COM_TOWER:
                    SPRITE(x, y, 40, 44, OWNER(below) + 3);
                    break;
                case TERRAIN_LAB:
                    SPRITE(x, y, 40, 48, OWNER(below) + 3);
                    break;
                case TERRAIN_SILO:
                    SPRITE(x, y, 40, 52, 3);
                    break;
                case TERRAIN_MINICANNON:
                    switch (OWNER(below)) {
                    case 0:
                        SPRITE(x, y, 40, 92, 9);
                        break;
                    case 1:
                        SPRITE(x, y, 40, 96, 9);
                        break;
                    case 2:
                        SPRITE(x, y, 40, 100, 9);
                        break;
                    case 3:
                        SPRITE(x, y, 40, 104, 9);
                        break;
                    }
                    break;
                }
            }
        }
    }

    /* The upper half of the tile goes to layer 1, one row up. */
    info = data_ov003_02365d10[code];
    sx = x - (SCROLL_X >> 4);
    sy = y - (SCROLL_Y >> 4);
    sy--;
    if (sx > 16 || sy > 12) {
        return;
    }
    if (sx >= 16) {
        sx -= 16;
        sy += 16;
    }
    terrain = info & 31;
    switch (terrain) {
    case 9:
    case TERRAIN_MINICANNON:
    case 25:
    case TERRAIN_BLACK_CANNON:
        PUT_TILE(layer1Map, CELL(sx, sy), data_ov003_02366110, code, 12);
        break;
    case TERRAIN_CITY:
        PUT_TILE(layer1Map, CELL(sx, sy), data_ov003_02366110, 450, 7);
        break;
    case TERRAIN_BASE:
        break;
    case TERRAIN_AIRPORT:
        PUT_TILE(layer1Map, CELL(sx, sy), data_ov003_02366110, 451, 7);
        break;
    case TERRAIN_PORT:
        PUT_TILE(layer1Map, CELL(sx, sy), data_ov003_02366110, 452, 7);
        break;
    case TERRAIN_COM_TOWER:
        PUT_TILE(layer1Map, CELL(sx, sy), data_ov003_02366110, 441, 7);
        break;
    case TERRAIN_LAB:
        PUT_TILE(layer1Map, CELL(sx, sy), data_ov003_02366110, 473, 7);
        break;
    case TERRAIN_HQ:
        switch (OWNER(info)) {
        case 1:
            code = 485;
            break;
        case 2:
            code = 490;
            break;
        case 3:
            code = 495;
            break;
        case 4:
            code = 500;
            break;
        }
        PUT_TILE_HQ(layer1Map, CELL(sx, sy), data_ov003_02366110, code, 7);
        break;
    default:
        PUT_TILE(layer1Map, CELL(sx, sy), data_ov003_02366110, code, 7);
        break;
    }

    switch (terrain) {
    case TERRAIN_MOUNTAIN:
        SPRITE(x, (u8)(y - 1), 10, 0, 0);
        break;
    case TERRAIN_WOOD:
        SPRITE(x, (u8)(y - 1), 10, 4, 1);
        break;
    case TERRAIN_HQ:
        owner = OWNER(info);
        SPRITE(x, (u8)(y - 1), 10, (owner - 1) * 4 + 8, owner + 3);
        break;
    case TERRAIN_CITY:
        SPRITE(x, (u8)(y - 1), 10, 28, OWNER(info) + 3);
        break;
    case TERRAIN_BASE:
        if ((owner = OWNER(info)) != 0) {
            SPRITE(x, (u8)(y - 1), 10, data_ov003_02364a84->unk_0b_2 * 4 + 56, owner + 3);
        }
        break;
    case TERRAIN_AIRPORT:
        SPRITE(x, (u8)(y - 1), 10, 36, OWNER(info) + 3);
        break;
    case TERRAIN_PORT:
        SPRITE(x, (u8)(y - 1), 10, 40, OWNER(info) + 3);
        break;
    case TERRAIN_COM_TOWER:
        SPRITE(x, (u8)(y - 1), 10, 44, OWNER(info) + 3);
        break;
    case TERRAIN_LAB:
        SPRITE(x, (u8)(y - 1), 10, 48, OWNER(info) + 3);
        break;
    case TERRAIN_SILO:
        SPRITE(x, (u8)(y - 1), 10, 52, 3);
        break;
    case TERRAIN_MINICANNON:
        if (object->unk_09_6) {
            switch (OWNER(info)) {
            case 0:
                SPRITE(x, (u8)(y - 1), 10, 92, 10);
                break;
            case 1:
                SPRITE(x, (u8)(y - 1), 10, 96, 10);
                break;
            case 2:
                SPRITE(x, (u8)(y - 1), 10, 100, 10);
                break;
            case 3:
                SPRITE(x, (u8)(y - 1), 10, 104, 10);
                break;
            }
        } else {
            switch (OWNER(info)) {
            case 0:
                SPRITE(x, (u8)(y - 1), 10, 92, 9);
                break;
            case 1:
                SPRITE(x, (u8)(y - 1), 10, 96, 9);
                break;
            case 2:
                SPRITE(x, (u8)(y - 1), 10, 100, 9);
                break;
            case 3:
                SPRITE(x, (u8)(y - 1), 10, 104, 9);
                break;
            }
        }
        break;
    }

    if (func_ov003_02361f38() == 0 && STATE->unk_1df_0 && mark == 0 && terrain == TERRAIN_WOOD) {
        Ov003_DrawTexturedQuad(x * 16 - SCROLL_X, (u8)(y - 1) * 16 - SCROLL_Y, 50, 1, 1, 4, 2, 0, 0);
    }
}
