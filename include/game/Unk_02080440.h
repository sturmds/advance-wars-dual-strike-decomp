#ifndef GAME_UNK_02080440_H
#define GAME_UNK_02080440_H

#include "types.h"
#include "game/player.h"
#include "game/map_unit.h"

/* Terrain and owner of every map tile, reached through
 * Unk_02080440_48.mapTiles. */
typedef struct MapTiles {
    u8 pad_00[0x1420];
    u8 tiles[0x4168 - 0x1420];         /* length unknown; bits 0-4 terrain id, bits 5-7 owner */
    u16 rowStart[1];                   /* length unknown; index into tiles of each row's first tile */
} MapTiles;

#define MAP_TILE_TERRAIN(t) ((t) & 0x1f)
#define MAP_TILE_OWNER(t)   (((t) & 0xe0) >> 5)

/* Terrain ids (MAP_TILE_TERRAIN) are in game/terrain.h. */

/* Reached through Unk_02080440.unk_48[i]->unk_1c4. */
typedef struct Unk_02080440_1c4 {
    u8 pad_00[0x1a];
    u16 mapWidth;                      /* in tiles */
    u16 mapHeight;
    u8 pad_1e[0x2a - 0x1e];
    u16 unk_2a;
    u8 pad_2c[0x30 - 0x2c];
    u32 unk_30;
    u16 mapId;                         /* index into Ov000_gMapHeaders */
    u8 pad_36[0x38 - 0x36];
    u8 fog;                            /* 1: Fog of War */
    u8 tileset;                        /* from MapHeader.unk_1a */
    u8 weather;                        /* WEATHER_* */
} Unk_02080440_1c4;

/* Unk_02080440_1c4.weather */
#define WEATHER_CLEAR 0
#define WEATHER_SNOW  1
#define WEATHER_RAIN  2
#define WEATHER_SAND  3

typedef struct Unk_02080440_48 {
    u8 pad_00[0x2];
    u16 currentPlayer;                 /* index into players; CO power code works on this player (likely whose turn it is) */
    u16 unk_04;                        /* compared with a map tile's owner bits (tile & 0xe0), unshifted */
    u16 currentUnitBase;               /* (currentPlayer - 1) * MAP_UNITS_PER_PLAYER: first slot in units */
    u8 pad_08[0x3e - 0x08];
    s16 unk_3e;
    s16 unk_40;
    s16 unk_42;
    s16 unk_44;
    u8 pad_46[0x58 - 0x46];
    u8 unk_58;
    u8 unk_59;
    u8 pad_5a[0x5d - 0x5a];
    u8 unk_5d;                         /* 0: no CO luck (good luck 10, bad luck 0) */
    u8 partnerBoostHuman;              /* nonzero: non-AI players get tag partner skills */
    u8 partnerBoostAi;                 /* nonzero: AI players get tag partner skills */
    u8 pad_60[0x78 - 0x60];
    s32 fundsPerProperty;              /* income from each city, base, airport, port and HQ */
    u32 unk_7c;
    u8 unk_80;                         /* from MapHeader.unk_30 */
    u8 unk_81;                         /* from MapHeader.unk_34 */
    u8 pad_82[0x88 - 0x82];
    u16 unk_88;
    u8 unk_8a;
    u8 pad_8b[0x92 - 0x8b];
    u8 unk_92[5];                      /* indexed 1 to 4, one per player?: 1 or 2 */
    u8 playerCos[5][2];                /* front and back CO id per player, 1-based (index 0 unused); 0: none */
    u8 pad_a1[0xba - 0xa1];
    u8 unk_ba;                         /* 1 turns off Scout and Eagle Eye, see func_020e8ae4 */
    u8 pad_bb[0x170 - 0xbb];
    u8 unk_170[12]; /* 12 values of 0-15, see PackNibblePairs */
    u8 pad_17c[0x1c4 - 0x17c];
    Unk_02080440_1c4 *unk_1c4;
    u8 pad_1c8[0x43d0 - 0x1c8];
    MapTiles *mapTiles;
    u8 pad_43d4[0x575c - 0x43d4];
    MapUnit *units;                    /* MAP_UNITS_PER_PLAYER slots per player */
    u8 pad_5760[0x5a58 - 0x5760];
    Player *players;                   /* 4 players */
} Unk_02080440_48;

/* Pointed to by data_027c027c (DTCM). */
typedef struct Unk_02080440 {
    u8 pad_00[0x48];
    Unk_02080440_48 *unk_48[1]; /* length unknown, indexed by data_027c0284 */
} Unk_02080440;

#endif
