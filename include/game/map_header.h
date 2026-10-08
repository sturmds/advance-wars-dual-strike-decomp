#ifndef GAME_MAP_HEADER_H
#define GAME_MAP_HEADER_H

#include "types.h"
#include "game/text_ref.h"

#define MAP_CAMPAIGN_FIRST 0xe0 /* campaign mission n is map id 0xe0 + n */
#define MAP_COUNT          0x10d /* entries in Ov000_gMapHeaders */

/* Map ids 0x101-0x10c have no header: the map lookups read them from 12
 * records of 27 bytes in main bss (data_022a6c84) instead. Probably maps
 * made in the Design Room. */
#define MAP_CUSTOM_FIRST 0x101
#define MAP_CUSTOM_COUNT 12
#define MAP_IS_CUSTOM(id) ((id) >= MAP_CUSTOM_FIRST && (id) < MAP_CUSTOM_FIRST + MAP_CUSTOM_COUNT)

/* Two flag bytes per slot at MapHeader +0x92, ORed into Player.unk_2f and
 * unk_30 by Map_ApplySlotFlags. 0xff in unk_0: none. */
typedef struct MapSlotFlags {
    u8 unk_0;
    u8 unk_1;
} MapSlotFlags;

/* One entry of the map header table (Ov000_gMapHeaders, ov000 0x022dbd28), indexed by map id.
 * Layout from romhacking.net doc 890 (RadioTails), checked against AWRE.
 * unk_ fields have a meaning in the document that the code has not
 * confirmed yet; their comments give it. */
typedef struct MapHeader {
    const void *unk_00;                /* map script */
    const void *unk_04;                /* "Terms" option script */
    const void *unk_08;                /* "Hint" option script */
    const void *unk_0c;                /* top-screen "Terms" script */
    u16 unk_10;                        /* map id for the top screen */
    u8 pad_12[0x14 - 0x12];
    TextRef name;
    u16 unk_18;
    u8 unk_1a;                         /* tileset */
    u8 unk_1b;                         /* weather */
    u8 fog;                            /* 1: Fog of War */
    u8 pad_1d[0x20 - 0x1d];
    u32 unk_20;
    u8 playerCount;
    u8 pad_25[0x26 - 0x25];
    u16 unk_26;                        /* "map case" */
    u8 unk_28[0x08];
    u16 unk_30[2];                     /* indexed by func_021039e4 */
    u16 unk_34[2];                     /* indexed by func_021039e4 */
    u8 unk_38[0x08];
    u32 unk_40;
    const void *mapData;               /* LZ77: width, height, then u16 tiles */
    const void *hardMapData;
    const u8 *units;                   /* pre-deployed units, 13-byte rows */
    const u8 *hardUnits;
    u8 co[4][2];                       /* front and back CO per player; 0x1c: player picks */
    u8 unk_5c[0x2c];
    u8 unk_88;                         /* army colour of slot 0 (neutral); army[] continues it, see Map_LoadArmyColours */
    u8 army[4];                        /* army colour per player */
    u8 unk_8d;
    u8 team[4];                        /* team per player */
    u8 unk_92[0x0a];
    u8 tileArmy;                       /* nonzero: army style of every tile but HQs, see Map_GetTileArmy */
    u8 pad_9d[0xa0 - 0x9d];
} MapHeader;

/* Fails to compile if a field change breaks the size the ROM uses. */
typedef char MapHeader_size_check[sizeof(MapHeader) == 0xa0 ? 1 : -1];

extern MapHeader Ov000_gMapHeaders[]; /* length unknown, indexed by map id */
extern MapHeader Ov000_gMapHeaders[];

#define ARMY_SLOT_COUNT 5         /* slot 0 neutral, then players 1-4 */
#define MAP_DESIGN_FIRST 0x101    /* ids 0x101-0x10c: player-made maps (a guess), kept in data_022a6c84 */
#define MAP_DESIGN_END 0x10d
#endif
