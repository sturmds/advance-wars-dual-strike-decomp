#ifndef GAME_UNIT_STATS_H
#define GAME_UNIT_STATS_H

#include "types.h"
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
#define ATTACK_NONE     0
#define ATTACK_DIRECT   1
#define ATTACK_INDIRECT 2

/* UnitStats.moveGroup, read from the table: infantry and mech FOOT,
 * recon, rockets and missiles TIRES, other ground units TREAD, planes and
 * copters AIR, ships SEA. */
#define MOVE_GROUP_FOOT  0x01
#define MOVE_GROUP_TIRES 0x02
#define MOVE_GROUP_TREAD 0x04
#define MOVE_GROUP_AIR   0x10
#define MOVE_GROUP_SEA   0x20

/* Fixed stats of one unit type, 0x6c bytes, in ov000 at 0x022f4fb8.
 * Table address from romhacking.net doc 890 (RadioTails); the fields
 * were read from AWRE and checked against warswiki for the Tank. */
typedef struct UnitStats {
    TextRef text[3];
    u16 costDiv10;                     /* cost in funds / 10 */
    u8 pad_0e[2];
    u8 movement;
    u8 ammo;
    u8 vision;
    u8 unk_13;
    u8 minRange;                       /* artillery 2, rockets 3; 0: no weapon (ATTACK_*) */
    u8 maxRange;
    u8 fuel;
    u8 unk_17[5];
    u8 unitClass;                      /* enum UnitClass in game/co_data.h */
    u8 pad_1d[2];
    u8 moveGroup;                      /* one MOVE_GROUP_* bit */
    u8 aiClass;                        /* AI_CLASS_*; the document calls this "unit class" */
    u8 pad_21[2];
    u8 chartDamage[2][32];             /* [weapon][damage chart column]: base damage in percent; 0: cannot hit */
    u8 pad_63[0x6c - 0x63];
} UnitStats;

typedef char UnitStats_size_check[sizeof(UnitStats) == 0x6c ? 1 : -1];

/* UnitStats.aiClass values: the group the AI unit action steps pick units
 * by. Read from the table; no unit type uses 3. */
#define AI_CLASS_OOZIUM         0
#define AI_CLASS_FOOT           1
#define AI_CLASS_TRANSPORT      2  /* APC, T-Copter */
#define AI_CLASS_INDIRECT       4  /* Artillery, Rockets, Missiles, Piperunner, Battleship, Carrier */
#define AI_CLASS_DIRECT         5
#define AI_CLASS_SEA_TRANSPORT  6  /* Black Boat, Lander */
#define AI_CLASS_BLACK_BOMB     7

extern UnitStats Ov000_gUnitStats[UNIT_TYPE_COUNT];

#endif
