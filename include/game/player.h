#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "types.h"

/* Bits of Player.frontCo and backCo. */
#define PLAYER_CO_ID_MASK      0x7f
#define PLAYER_POWER_ACTIVE(w) (((w) >> 10) & 3) /* enum CoPowerState */

/* Bitfield view of Player.frontCo and backCo. The fields stay u32 so
 * other code keeps its shifts; cast to this where the game code used
 * bitfields (the reads and writes compile differently). */
typedef struct PlayerCoStatus {
    u32 co : 7;                        /* enum CoId */
    u32 unk_7 : 1;
    u32 powerReady : 2;
    u32 powerActive : 2;               /* enum CoPowerState */
    u32 unk_12 : 2;
    u32 powerUses : 4;                 /* stops at 10 */
    u32 unk_18 : 3;
    u32 charge : 11;                   /* power meter */
} PlayerCoStatus;

#define PLAYER_CO_STATUS(word) ((PlayerCoStatus *)&(word))

/* Player.front and back: the same words as frontCo and backCo, as bit
 * fields. Game code reads them this way; the u32 view is kept for code that
 * uses the PLAYER_ macros. */
typedef struct PlayerCo {
    u32 id : 8;                        /* enum CoId in bits 0-6; mask with PLAYER_CO_ID_MASK */
    u32 unk_8 : 2;
    u32 powerState : 2;                /* enum CoPowerState */
    u32 unk_12 : 20;
} PlayerCo;

/* Values of Player.controller. */
#define PLAYER_CONTROLLER_NONE  0
#define PLAYER_CONTROLLER_HUMAN 1
#define PLAYER_CONTROLLER_AI    2

/* One player slot, 0x98 bytes, in the array at Unk_02080440_48.players:
 * 5 slots, slot 0 neutral, then players 1-4. Layout from romhacking.net doc 890 (RadioTails);
 * fields named here have been seen in code, unk_ fields carry the
 * document's reading as a comment. */
typedef struct Player {
    s32 funds;
    u32 unk_04;                        /* money spent */
    u32 unk_08;                        /* income */
    u32 unk_0c;                        /* damage taken, in funds */
    u8 unk_10[5];                      /* bases, cities, airports, ports, towers owned */
    u8 unk_15;                         /* total properties */
    u8 unk_16;                         /* defeated */
    u8 pad_17[2];
    u8 unk_19;                         /* army colour */
    u8 controller;                     /* PLAYER_CONTROLLER_* */
    u8 unk_1b;
    u16 unk_1c;                        /* kills this turn */
    u16 unk_1e;                        /* most kills in one turn */
    u8 unk_20[5];
    u8 unk_25;                         /* tag power active: 1 front, 2 back */
    s16 firepowerBonus;                /* set to 10 while a power is on */
    s16 defenseBonus;                  /* set to 10 while a power is on */
    s8 terrainDefense;                 /* added to terrain defense; enemy Sonja-style star cuts make it negative */
    u8 moveChart;                      /* enum MoveChart: index into CoPowerBlock.movementCost */
    u8 team;
    u8 armyStyle;
    u8 unk_2e;                         /* AI behaviour */
    u8 unk_2f;                         /* ORed with MapHeader.slotFlags[slot].unk_0 at map start */
    u8 unk_30;                         /* ORed with MapHeader.slotFlags[slot].unk_1 */
    u8 pad_31[0x6c - 0x31];
    union {
        u32 frontCo;                   /* CO id and power state, see PLAYER_ macros */
        PlayerCo front;
    };
    union {
        u32 backCo;
        PlayerCo back;
    };
    u32 skills[2][4];                  /* front CO, back CO; ids in game/skills.h */
    u32 unk_94;                        /* Time Survival timer, frames */
} Player;

typedef char Player_size_check[sizeof(Player) == 0x98 ? 1 : -1];

#endif
