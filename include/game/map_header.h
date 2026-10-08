#ifndef GAME_MAP_HEADER_H
#define GAME_MAP_HEADER_H

#include "types.h"
#include "game/text_ref.h"

#define MAP_CAMPAIGN_FIRST 0xe0 /* campaign mission n is map id 0xe0 + n */

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
    u16 pad_12;
    TextRef name;
    u16 unk_18;
    u8 unk_1a;                         /* tileset */
    u8 unk_1b;                         /* weather */
    u8 fog;                            /* 1: Fog of War */
    u8 pad_1d[3];
    u32 unk_20;
    u16 playerCount;
    u16 unk_26;                        /* "map case" */
    u8 unk_28[0x18];
    u32 unk_40;
    const void *mapData;               /* LZ77: width, height, then u16 tiles */
    const void *hardMapData;
    const u8 *units;                   /* pre-deployed units, 13-byte rows */
    const u8 *hardUnits;
    u8 co[4][2];                       /* front and back CO per player; 0x1c: player picks */
    u8 unk_5c[0x2d];
    u8 army[4];                        /* army colour per player */
    u8 unk_8d;
    u8 team[4];                        /* team per player */
    u8 unk_92[0x0e];
} MapHeader;

/* Fails to compile if a field change breaks the size the ROM uses. */
typedef char MapHeader_size_check[sizeof(MapHeader) == 0xa0 ? 1 : -1];

#endif
