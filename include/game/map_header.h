#ifndef GAME_MAP_HEADER_H
#define GAME_MAP_HEADER_H

#include "types.h"
#include "game/player.h"
#include "game/text_ref.h"
#include "game/predeployed_unit.h"

struct MapScript;
struct EventScriptCommand;

#define MAP_CAMPAIGN_FIRST 0xe0 /* campaign mission n is map id 0xe0 + n */
#define MAP_COUNT 0x10d         /* entries in Ov000_gMapHeaders */

/* The two campaign missions with Auto CO: the battle menu lets the player
 * play the other front instead of leaving it to the AI. */
#define MAP_LIGHTNING_STRIKES (MAP_CAMPAIGN_FIRST + 10)
#define MAP_RING_OF_FIRE (MAP_CAMPAIGN_FIRST + 21)

/* Campaign missions where every side counts as Allied Nations
 * (Player_IsAlliedNations, with MAP_LIGHTNING_STRIKES). */
#define MAP_JAKES_TRIAL (MAP_CAMPAIGN_FIRST + 0)
#define MAP_VERDANT_HILLS (MAP_CAMPAIGN_FIRST + 12)
#define MAP_MUCK_AMOK (MAP_CAMPAIGN_FIRST + 16)
#define MAP_DARK_AMBITION (MAP_CAMPAIGN_FIRST + 19)

/* Map ids 0x101-0x10c have no header: the map lookups read them from 12
 * records of 27 bytes in main bss (data_022a6c84) instead. Probably maps
 * made in the Design Room. */
#define MAP_CUSTOM_FIRST 0x101
#define MAP_CUSTOM_COUNT 12
#define MAP_IS_CUSTOM(id) \
  ((id) >= MAP_CUSTOM_FIRST && (id) < MAP_CUSTOM_FIRST + MAP_CUSTOM_COUNT)

/* MapHeader.mapCase: which menu offers the map. The map select lists one
 * case per list kind (MapSelect_ListKindToMapCase). The unpacked maps keep one
 * directory per group of cases. */
enum MapCase {
  MAP_CASE_CAMPAIGN = 0, /* campaign missions */
  MAP_CASE_CAMPAIGN_SECOND =
      1,                     /* the second map of a two-map campaign mission */
  MAP_CASE_VERSUS_FIRST = 2, /* cases 2-6: the Versus map lists */
  MAP_CASE_VERSUS_LAST = 6,
  MAP_CASE_WAR_ROOM = 7,
  MAP_CASE_WAR_ROOM_DS =
      8, /* listed with case 7 by MapSelect_FillList; names end in "DS" */
  MAP_CASE_SURVIVAL = 9, /* the maps of gSurvivalMapsMode0-5 */
  MAP_CASE_DESIGN = 10,  /* Design Room slots, no map data */
  MAP_CASE_NONE = 0xffff /* map 0 */
};
#define MAP_CASE_COUNT (MAP_CASE_DESIGN + 1)

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
  struct MapScript* script; /* the map's event trigger lists (MapScript_Get) */
  const struct EventScriptCommand*
      termsScript; /* "Terms" option event script; NULL: none */
  const struct EventScriptCommand*
      hintScript; /* "Hint" option event script; NULL: none */
  const char*
      extraObjTileFile; /* /bmap file name (0a5 or 0a6) loaded by Map_LoadExtraObjTiles, NULL on most maps */
  u16 variantMapIds
      [2]; /* variant n of this map is map variantMapIds[n - 1]; [0] also links the second map of a two-map battle */
  TextRef name;
  u16 unk_18;
  u8 tileset;
  u8 weather;
  u8 fog; /* 1: Fog of War */
  u8 pad_1d[0x20 - 0x1d];
  const u8* coList; /* COs offered on this map, see LoadMapCoList */
  u8 playerCount;
  u8 pad_25[0x26 - 0x25];
  u16 mapCase; /* enum MapCase */
  u8 unk_28[0x08];
  u16 unk_30[2]; /* indexed by Campaign_IsHardMode */
  u16 unk_34[2]; /* indexed by Campaign_IsHardMode */
  u8 unk_38[0x08];
  u8 unk_40;
  u8 unk_41;
  u8 unk_42; /* bit 0: see func_0209dec0 */
  u8 unk_43;
  /* Map_LoadData reads mapData and hardMapData as a two-entry array
     * indexed by Campaign_IsHardMode. They stay separate fields: declared as
     * mapData[2], Map_LoadData no longer matches. */
  const void* mapData; /* LZ77: width, height, then u16 tiles */
  const void* hardMapData;
  const PredeployedUnit* units; /* pre-deployed units; NULL: none */
  const PredeployedUnit*
      hardUnits; /* read with units as a two-entry array, indexed by Campaign_IsHardMode */
  u8 co
      [ARMY_SLOT_COUNT]
      [PLAYER_CO_COUNT]; /* front and back CO per slot, row 0 unused; CO_PLAYER_PICKS: the player picks */
  u8 unk_5e[0x60 - 0x5e];
  const u8* skills
      [ARMY_SLOT_COUNT]
      [PLAYER_CO_COUNT]; /* skill ids for computer players per slot and CO, up to 4 ended by 0; NULL: none */
  u8 army[ARMY_SLOT_COUNT]; /* army colour (palette) per slot */
  u8 team[ARMY_SLOT_COUNT]; /* team per slot */
  MapSlotFlags slotFlags[ARMY_SLOT_COUNT];
  u8 tileArmy; /* nonzero: army style (sprite set) of every tile but HQs, see Map_GetTileArmy */
  u8 pad_9d[0xa0 - 0x9d];
} MapHeader;

/* Fails to compile if a field change breaks the size the ROM uses. */
typedef char MapHeader_size_check[sizeof(MapHeader) == 0xa0 ? 1 : -1];

extern MapHeader Ov000_gMapHeaders[]; /* length unknown, indexed by map id */
extern MapHeader Ov000_gMapHeaders[];

extern u8 gArmyColours[ARMY_SLOT_COUNT]; /* army colour of each slot */
#define MAP_DESIGN_FIRST \
  0x101 /* ids 0x101-0x10c: player-made maps (a guess), kept in data_022a6c84 */
#define MAP_DESIGN_END 0x10d
#endif
