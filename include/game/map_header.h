#ifndef GAME_MAP_HEADER_H
#define GAME_MAP_HEADER_H

#include "types.h"
#include "game/text_ref.h"
#include "game/predeployed_unit.h"

struct MapScript;

#define MAP_CAMPAIGN_FIRST 0xe0 /* campaign mission n is map id 0xe0 + n */
#define MAP_COUNT          0x10d /* entries in Ov000_gMapHeaders */

/* The two campaign missions with Auto CO: the battle menu lets the player
 * play the other front instead of leaving it to the AI. */
#define MAP_LIGHTNING_STRIKES (MAP_CAMPAIGN_FIRST + 10)
#define MAP_RING_OF_FIRE      (MAP_CAMPAIGN_FIRST + 21)

/* Campaign missions where every side counts as Allied Nations
 * (Player_IsAlliedNations, with MAP_LIGHTNING_STRIKES). */
#define MAP_JAKES_TRIAL   (MAP_CAMPAIGN_FIRST + 0)
#define MAP_VERDANT_HILLS (MAP_CAMPAIGN_FIRST + 12)
#define MAP_MUCK_AMOK     (MAP_CAMPAIGN_FIRST + 16)
#define MAP_DARK_AMBITION (MAP_CAMPAIGN_FIRST + 19)

/* Map ids 0x101-0x10c have no header: the map lookups read them from 12
 * records of 27 bytes in main bss (data_022a6c84) instead. Probably maps
 * made in the Design Room. */
#define MAP_CUSTOM_FIRST 0x101
#define MAP_CUSTOM_COUNT 12
#define MAP_IS_CUSTOM(id) ((id) >= MAP_CUSTOM_FIRST && (id) < MAP_CUSTOM_FIRST + MAP_CUSTOM_COUNT)

/* Two flag bytes per player slot at MapHeader.slotFlags, ORed into
 * Player.unk_2f and unk_30 by Map_ApplySlotFlags. 0xff in unk_0: none. */
typedef struct MapSlotFlags {
    u8 unk_0;
    u8 unk_1;
} MapSlotFlags;

/* One entry of the map header table (Ov000_gMapHeaders, ov000 0x022dbd28), indexed by map id.
 * Layout from romhacking.net doc 890 (RadioTails), checked against AWRE.
 * unk_ fields have a meaning in the document that the code has not
 * confirmed yet; their comments give it. Arrays of 5 are indexed by player
 * slot: 0 neutral, then players 1-4. */
typedef struct MapHeader {
    struct MapScript *script;          /* the map's event trigger lists (MapScript_Get) */
    const void *termsScript;           /* "Terms" option script; NULL: none */
    const void *hintScript;            /* "Hint" option script; NULL: none */
    const char *extraObjTileFile;      /* /bmap file name (0a5 or 0a6) loaded by Map_LoadExtraObjTiles, NULL on most maps */
    u16 variantMapIds[2];              /* variant n of this map is map variantMapIds[n - 1]; [0] also links the second map of a two-map battle */
    TextRef name;
    u16 unk_18;
    u8 tileset;
    u8 weather;
    u8 fog;                            /* 1: Fog of War */
    u8 pad_1d[0x20 - 0x1d];
    const u8 *coList;                  /* COs offered on this map, see LoadMapCoList */
    u8 playerCount;
    u8 pad_25[0x26 - 0x25];
    u16 unk_26;                        /* "map case" */
    u8 unk_28[0x08];
    u16 unk_30[2];                     /* indexed by Campaign_IsHardMode */
    u16 unk_34[2];                     /* indexed by Campaign_IsHardMode */
    u8 unk_38[0x08];
    u8 unk_40;
    u8 unk_41;
    u8 unk_42;                         /* bit 0: see func_0209dec0 */
    u8 unk_43;
    /* Map_LoadData reads mapData and hardMapData as a two-entry array
     * indexed by Campaign_IsHardMode. They stay separate fields: declared as
     * mapData[2], Map_LoadData no longer matches. */
    const void *mapData;               /* LZ77: width, height, then u16 tiles */
    const void *hardMapData;
    const PredeployedUnit *units;      /* pre-deployed units; NULL: none */
    const PredeployedUnit *hardUnits;  /* read with units as a two-entry array, indexed by Campaign_IsHardMode */
    u8 co[5][2];                       /* front and back CO per slot, row 0 unused; 0x1c: player picks */
    u8 unk_5e[0x60 - 0x5e];
    const u8 *skills[5][2];            /* skill ids for computer players per slot and CO, up to 4 ended by 0; NULL: none */
    u8 army[5];                        /* army colour (palette) per slot */
    u8 team[5];                        /* team per slot */
    MapSlotFlags slotFlags[5];
    u8 tileArmy;                       /* nonzero: army style (sprite set) of every tile but HQs, see Map_GetTileArmy */
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
