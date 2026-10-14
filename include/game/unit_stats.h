#ifndef GAME_UNIT_STATS_H
#define GAME_UNIT_STATS_H

#include "types.h"
#include "game/terrain.h"
#include "game/text_ref.h"

/* Unit type ids: index into Ov000_gUnitStats. */
enum UnitType {
  UNIT_NONE,
  UNIT_INFANTRY,
  UNIT_MECH,
  UNIT_MEDIUM_TANK,
  UNIT_MEGATANK,
  UNIT_TANK,
  UNIT_RECON,
  UNIT_APC,
  UNIT_NEOTANK,
  UNIT_PIPERUNNER,
  UNIT_ARTILLERY,
  UNIT_ROCKETS,
  UNIT_STEALTH,
  UNIT_BLACK_BOMB,
  UNIT_ANTI_AIR,
  UNIT_MISSILES,
  UNIT_FIGHTER,
  UNIT_BOMBER,
  UNIT_BLACK_BOAT,
  UNIT_B_COPTER,
  UNIT_T_COPTER,
  UNIT_BATTLESHIP,
  UNIT_CRUISER,
  UNIT_LANDER,
  UNIT_SUB,
  UNIT_CARRIER,
  UNIT_OOZIUM,
  UNIT_TYPE_COUNT
};

/* UnitStats.minRange, read as the kind of attack: no weapon, direct
 * (range 1), or indirect (2 or more). */
#define ATTACK_NONE 0
#define ATTACK_DIRECT 1
#define ATTACK_INDIRECT 2

/* UnitType_GetAttackKind: how a unit type attacks, from its range after
 * CO bonuses. */
#define ATTACK_KIND_DIRECT 0
#define ATTACK_KIND_INDIRECT 1
#define ATTACK_KIND_NONE 2
#define ATTACK_KIND_BLACK_BOMB 3
#define ATTACK_KIND_OOZIUM 4

/* UnitStats.moveGroup, read from the table: infantry and mech FOOT,
 * recon, rockets and missiles TIRES, other ground units TREAD, planes and
 * copters AIR, ships SEA. */
#define MOVE_GROUP_FOOT 0x01
#define MOVE_GROUP_TIRES 0x02
#define MOVE_GROUP_TREAD 0x04
#define MOVE_GROUP_AIR 0x10
#define MOVE_GROUP_SEA 0x20

/* UnitStats.categoryMask bits (and targetMask), read from the table. */
#define UNIT_CATEGORY_PLANE 0x01
#define UNIT_CATEGORY_COPTER 0x02

/* UnitStats.chartDamage columns: the target's unit type, except a dived
 * Sub or Stealth, which have their own columns. */
#define DAMAGE_COLUMN_DIVED_SUB 27
#define DAMAGE_COLUMN_DIVED_STEALTH 28
#define DAMAGE_COLUMN_TILE 29 /* damage-chart target kind 6, a tile with hp */
#define DAMAGE_COLUMN_OBJECT_A \
  30 /* map objects; which kinds use which is unconfirmed */
#define DAMAGE_COLUMN_OBJECT_B 31

/* Fixed stats of one unit type, 0x6c bytes, in ov000 at 0x022f4fb8.
 * Table address from romhacking.net doc 890 (RadioTails); the fields
 * were read from AWRE and checked against warswiki for the Tank. */
typedef struct UnitStats {
  TextRef text[3];
  u16 costDiv10; /* cost in funds / 10 */
  u16 ammoCost; /* guess: funds per shown HP for each ammo point (Unit_Rearm) */
  u8 movement;
  u8 ammo;
  u8 vision;
  u8 chargePerHp; /* power meter charge per shown HP lost (UnitType_GetChargePerHp); Infantry 2, Megatank 11 */
  u8 minRange;    /* artillery 2, rockets 3; 0: no weapon (ATTACK_*) */
  u8 maxRange;
  u8 fuel;
  u8 unk_17; /* 1 for Infantry to Recon, Neotank, Fighter, B-Copter, Cruiser; 0 for the rest */
  const u8*
      carryTypes; /* NULL for non-transports; [type + 1] nonzero: takes that unit type aboard */
  u8 unitClass; /* enum UnitClass in game/co_data.h */
  u8 unk_1d; /* effect id the destroy task plays on the unit tile; 24: one per army colour (UnitDestroyTask_PlayEffectAndRemove) */
  u8 moveType; /* row of a CoPowerBlock.movementCost chart (32 terrain ids per row) */
  u8 moveGroup; /* one MOVE_GROUP_* bit */
  u8 aiClass;   /* AI_CLASS_*; the document calls this "unit class" */
  u8 targetMask; /* categories of unit this type can attack, ANDed with categoryMask */
  u8 categoryMask; /* the category this type belongs to, one bit */
  u8 chartDamage
      [2]
      [32]; /* [weapon][damage chart column]: base damage in percent; 0: cannot hit */
  u8 pad_63;
  const s8*
      repairTerrain; /* per terrain id: nonzero where this type is repaired (ground: city, HQ, base; air: airport; sea: port) */
  const s8*
      dailyFuel; /* per terrain id: fuel used at the start of each day (Fighter 5, Battleship 1) */
} UnitStats;

typedef char UnitStats_size_check[sizeof(UnitStats) == 0x6c ? 1 : -1];

/* UnitStats.aiClass values: the group the AI unit action steps pick units
 * by. Read from the table; no unit type uses 3. */
#define AI_CLASS_OOZIUM 0
#define AI_CLASS_FOOT 1
#define AI_CLASS_TRANSPORT 2 /* APC, T-Copter */
#define AI_CLASS_INDIRECT \
  4 /* Artillery, Rockets, Missiles, Piperunner, Battleship, Carrier */
#define AI_CLASS_DIRECT 5
#define AI_CLASS_SEA_TRANSPORT 6 /* Black Boat, Lander */
#define AI_CLASS_BLACK_BOMB 7

extern UnitStats Ov000_gUnitStats[UNIT_TYPE_COUNT];

/* Tables of Ov000_gUnitStats that are all zero, so in .bss. */
extern s8 data_ov000_02350340[TERRAIN_ID_COUNT]; /* dailyFuel */
extern s8 data_ov000_02350360[TERRAIN_ID_COUNT]; /* dailyFuel */
extern s8 data_ov000_02350380[TERRAIN_ID_COUNT]; /* repairTerrain */

#endif
