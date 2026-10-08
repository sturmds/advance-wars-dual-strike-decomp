#ifndef GAME_UNK_02080440_H
#define GAME_UNK_02080440_H

#include "types.h"
#include "game/player.h"

/* Terrain and owner of every map tile, reached through
 * Unk_02080440_48.mapTiles. */
typedef struct MapTiles {
    u8 pad_00[0x1420];
    u8 tiles[0x4168 - 0x1420];         /* length unknown; bits 0-4 terrain id, bits 5-7 owner */
    u16 rowStart[1];                   /* length unknown; index into tiles of each row's first tile */
} MapTiles;

#define MAP_TILE_TERRAIN(t) ((t) & 0x1f)
#define MAP_TILE_OWNER(t)   (((t) & 0xe0) >> 5)

/* Reached through Unk_02080440.unk_48[i]->unk_1c4. */
typedef struct Unk_02080440_1c4 {
    u8 pad_00[0x1a];
    u16 mapWidth;                      /* in tiles */
    u16 mapHeight;
    u8 pad_1e[0x2a - 0x1e];
    u16 unk_2a;
    u8 pad_2c[0x34 - 0x2c];
    u16 mapId;                         /* index into Ov000_gMapHeaders */
    u8 pad_36[0x3a - 0x36];
    u8 weather;                        /* WEATHER_* */
} Unk_02080440_1c4;

/* Unk_02080440_1c4.weather */
#define WEATHER_CLEAR 0
#define WEATHER_SNOW  1
#define WEATHER_RAIN  2
#define WEATHER_SAND  3

typedef struct Unk_02080440_48 {
    u8 pad_00[2];
    u16 currentPlayer;                 /* index into players; CO power code works on this player (likely whose turn it is) */
    u8 pad_04[0x3e - 0x04];
    s16 unk_3e;
    s16 unk_40;
    s16 unk_42;
    s16 unk_44;
    u8 pad_46[0x170 - 0x46];
    u8 unk_170[12]; /* 12 values of 0-15, see PackNibblePairs */
    u8 pad_17c[0x1c4 - 0x17c];
    Unk_02080440_1c4 *unk_1c4;
    u8 pad_1c8[0x43d0 - 0x1c8];
    MapTiles *mapTiles;
    u8 pad_43d4[0x5a58 - 0x43d4];
    Player *players;                   /* 4 players */
} Unk_02080440_48;

/* Pointed to by data_027c027c (DTCM). */
typedef struct Unk_02080440 {
    u8 pad_00[0x48];
    Unk_02080440_48 *unk_48[1]; /* length unknown, indexed by data_027c0284 */
} Unk_02080440;

#endif
