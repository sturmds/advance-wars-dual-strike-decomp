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

/* UnitStats.attackType */
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
    u8 attackType;                     /* ATTACK_* */
    u8 maxRange;
    u8 fuel;
    u8 unk_17[5];
    u8 unitClass;                      /* enum UnitClass in game/co_data.h */
    u8 pad_1d[2];
    u8 moveGroup;                      /* one MOVE_GROUP_* bit */
    u8 unk_20;                         /* the document calls this "unit class"; the code uses unitClass */
    u8 pad_21[0x6c - 0x21];
} UnitStats;

typedef char UnitStats_size_check[sizeof(UnitStats) == 0x6c ? 1 : -1];

extern UnitStats Ov000_gUnitStats[UNIT_TYPE_COUNT];

#endif
