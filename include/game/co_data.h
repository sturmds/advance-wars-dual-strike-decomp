#ifndef GAME_CO_DATA_H
#define GAME_CO_DATA_H

#include "types.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/text_ref.h"

/* CO ids, which are also indexes into the CO table. */
enum CoId {
  CO_NONE,
  CO_NELL,
  CO_ANDY,
  CO_MAX,
  CO_OLAF,
  CO_SAMI,
  CO_GRIT,
  CO_KANBEI,
  CO_SONJA,
  CO_EAGLE,
  CO_DRAKE,
  CO_VON_BOLT,
  CO_JUGGER,
  CO_LASH,
  CO_KOAL,
  CO_HAWKE,
  CO_HACHI,
  CO_COLIN,
  CO_JESS,
  CO_SENSEI,
  CO_JAKE,
  CO_RACHEL,
  CO_SASHA,
  CO_JAVIER,
  CO_GRIMM,
  CO_KINDLE,
  CO_FLAK,
  CO_ADDER,
  CO_COUNT,
  CO_PLAYER_PICKS = CO_COUNT, /* in map headers: the player chooses */
  /* Bit 7 marks a clone: one of Black Hole's copies of a CO, met in the
   * campaign (Dark Ambition, Pincer Strike, Ring of Fire, Surrounded). The
   * code masks it off with CO_ID_MASK to index CO tables. A clone has no
   * experience, picture variant 0 and palette CO_CLONE_PALETTE. */
  CO_CLONE = 0x80,
  CO_ID_MASK = 0x7f,
  CO_CLONE_ANDY = CO_CLONE | CO_ANDY,
  CO_CLONE_OLAF = CO_CLONE | CO_OLAF,
  CO_CLONE_KANBEI = CO_CLONE | CO_KANBEI,
  CO_CLONE_DRAKE = CO_CLONE | CO_DRAKE
};

/* The palette a clone's pictures use, in place of the saved variant. */
#define CO_CLONE_PALETTE 7

/* Which power block of a CO record is in effect. */
enum CoPowerState {
  POWER_NONE, /* day-to-day */
  POWER_CO,
  POWER_SUPER,
  POWER_STATE_COUNT
};

/* Index into CoPowerBlock.statRows. UnitStats.unitClass holds one of
 * UNIT_CLASS_FOOT to UNIT_CLASS_NAVY, or UNIT_CLASS_ALL for Oozium. */
enum UnitClass {
  UNIT_CLASS_FOOT,
  UNIT_CLASS_DIRECT_VEHICLE,
  UNIT_CLASS_INDIRECT_VEHICLE,
  UNIT_CLASS_PLANE,
  UNIT_CLASS_COPTER,
  UNIT_CLASS_NAVY,
  UNIT_CLASS_ALL, /* no CO uses this row */
  UNIT_CLASS_DIRECT_COMBAT,
  UNIT_CLASS_INDIRECT_COMBAT,
  UNIT_CLASS_TRANSPORT,
  UNIT_CLASS_EXPLODES, /* Black Bomb, Oozium */
  UNIT_CLASS_COUNT
};

/* Bits of CoPowerBlock.skills1 and skills2 (retail values). */
#define CO_SKILL1_HIDDEN_HP 0x01
#define CO_SKILL1_DEPLOY_FROM_CITIES 0x02
#define CO_SKILL1_COUNTER_FIRST 0x04
#define CO_SKILL1_PIERCING_VISION 0x08
#define CO_SKILL1_DOUBLE_TERRAIN 0x10
#define CO_SKILL1_SNOW_FIREPOWER 0x20
#define CO_SKILL1_AIR_FUEL_SAVING 0x40
#define CO_SKILL2_DAMAGE_TO_FUNDS 0x04
#define CO_SKILL2_INDIRECT_DEFENSE_1 0x08
#define CO_SKILL2_INDIRECT_DEFENSE_2 0x10
#define CO_SKILL2_INDIRECT_DEFENSE_3 0x20
#define CO_SKILL2_FUNDS_PER_PROPERTY 0x40
#define CO_SKILL2_TERRAIN_FIREPOWER 0x80

/* Index into CoPowerBlock.movementCost, chosen per player by MovementCost
 * from the front CO's skills. */
enum MoveChart {
  MOVE_CHART_DEFAULT,
  MOVE_CHART_PRAIRIE_DOG,
  MOVE_CHART_PATHFINDER,
  MOVE_CHART_BOTH,
  MOVE_CHART_COUNT
};

/* Changes a CO makes to one unit class. Each value is an offset; 0 means
 * no change. Firepower and defense are in percent. */
typedef struct CoStatRow {
  s16 firepower;
  s16 defense;
  s16 movement;
  s16 range;
} CoStatRow;

/* Quotes and power name for one tag pair, reached from CoTagPartner. */
typedef struct CoTagRecord {
  TextRef winQuote1;
  TextRef partnerWinQuote1;
  TextRef winQuote2;
  TextRef partnerWinQuote2;
  TextRef tagPowerName;
} CoTagRecord;

typedef struct CoTagPartner {
  u16 co;
  u16 stars; /* 1-3: +5, +10, +15 % luck */
  const CoTagRecord* record;
} CoTagPartner;

/* Where the parameters are not known yet, the pointer types leave them
 * unspecified. */
typedef void (*CoPowerFunc)();
typedef BOOL (*CoUnitFilterFunc)();
typedef void (*CoUnitBonusFunc)(MapUnit* unit);
typedef void (*CoCalcBonusFunc)();

/* One CO in one power state. */
typedef struct CoPowerBlock {
  TextRef name;         /* zero in the day-to-day block */
  CoPowerFunc activate; /* run when the power starts */
  u8 skills1;           /* CO_SKILL1_* */
  u8 skills2;           /* CO_SKILL2_* */
  u8 unk_0a;
  u8 unk_0b;
  u32 unk_0c[3];
  s16 visionBonus;
  s16 repairBonus;
  s16 goodLuck; /* luck roll maximum */
  s16 badLuck;  /* bad luck roll maximum */
  s16 counterBonus;
  s16 costPercent; /* unit cost offset */
  s16 captureRate;
  s16 towerFirepower; /* per Com Tower */
  s16 towerDefense;   /* per Com Tower */
  s16 terrainStarCut;
  const s8* terrainFirepower; /* one value per terrain id */
  u8 highlightEffect;
  u8 highlightColor;
  u8 pad_32[2];
  CoUnitFilterFunc affectsUnit; /* which units the power affects */
  CoUnitBonusFunc unitBonus;    /* applied to units on activation */
  CoCalcBonusFunc calcBonus;
  u16 sounds[2];
  const u8* movementCost[4]; /* default, Prairie Dog, Pathfinder, both */
  const CoStatRow* statRows[UNIT_CLASS_COUNT];
} CoPowerBlock;

/* All zero, so in .bss: the terrainFirepower table of the blocks with no
 * terrain bonus, and the statRows entry for no change (ov000 and main). */
extern s8 Ov000_gNoBoost[TERRAIN_ID_COUNT];
extern CoStatRow data_0216e03c;

/* CoRecord.nation of the Black Hole COs. */
#define NATION_BLACK_HOLE 5
/* CoRecord.nation of Andy, Max and Sami. */
#define NATION_ORANGE_STAR 1

/* One entry of gCoTable (main 0x0215360c), indexed by CO id.
 * Layout from romhacking.net doc 890 (RadioTails), checked against AWRE. */
typedef struct CoRecord {
  TextRef name;
  TextRef profiles[4]; /* about, day-to-day, CO Power, Super */
  u16 theme;
  u8 pad_16[6];
  u32 powerStars; /* CO Power cost in stars */
  u32 superStars; /* Super CO Power cost in stars */
  u8 unk_24;      /* always 1 */
  u8 armyStyle; /* sprite set of the CO's army, 0-4: OS, BM, GE, YC, BH (Player.armyStyle is this + 1) */
  u8 nation;    /* 1-5 (5: NATION_BLACK_HOLE); picks power themes */
  u8 pad_27;
  TextRef winQuotes[3];
  TextRef switchQuotes[2];
  TextRef powerQuotes[6];
  u32 unk_54;
  const void* nameGraphics[5]; /* compressed; EN, FR, DE, ES, IT */
  CoTagPartner tagPartners[3];
  u8 tagFirepower[CO_COUNT]; /* percent, 100 = no change; by partner CO id */
  CoPowerBlock blocks[POWER_STATE_COUNT];
} CoRecord;

extern CoRecord gCoTable[CO_COUNT];

/* Address of gCoTable[co].field, written so that the compiler folds the
 * field's offset into gCoTable's address and adds co * sizeof(CoRecord) to
 * that, as the original code does (needed to match). The ROM has a symbol
 * for each such field of gCoTable[0] (config/AWRE/aliases.txt); those hold
 * retail offsets, so code reads the fields through this macro instead. */
#define CO_TABLE_FIELD(co, field) ((void*)&((CoRecord*)&gCoTable[0].field)[co])

/* Fails to compile if a field change breaks the sizes the ROM uses. */
typedef char CoPowerBlock_size_check[sizeof(CoPowerBlock) == 0x80 ? 1 : -1];
/* tagFirepower has one byte per CO id (retail: 28, a record of 0x220
 * bytes); the power blocks follow it, word aligned. */
typedef char
    CoRecord_tag_check[offsetof(CoRecord, tagFirepower) == 0x84 ? 1 : -1];
typedef char
    CoRecord_size_check[sizeof(CoRecord) == ((0x84 + CO_COUNT + 3) & ~3) +
                                                POWER_STATE_COUNT * 0x80
                            ? 1
                            : -1];

#endif
