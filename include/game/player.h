#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "types.h"

/* Bits of Player.frontCo and backCo. */
#define PLAYER_CO_ID_MASK      0x7f
#define PLAYER_POWER_ACTIVE(w) (((w) >> 10) & 3) /* enum CoPowerState */

/* Values of Player.controller. */
#define PLAYER_CONTROLLER_NONE  0
#define PLAYER_CONTROLLER_HUMAN 1
#define PLAYER_CONTROLLER_AI    2

/* One of the four players, 0x98 bytes, in the array at
 * Unk_02080440_48.players. Layout from romhacking.net doc 890 (RadioTails);
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
    s8 unk_2a;                         /* terrain defense adjustment */
    u8 unk_2b;                         /* movement chart */
    u8 unk_2c;                         /* team */
    u8 unk_2d;
    u8 unk_2e;                         /* AI behaviour */
    u8 pad_2f[0x6c - 0x2f];
    u32 frontCo;                       /* CO id and power state, see PLAYER_ macros */
    u32 backCo;
    u32 skills[2][4];                  /* front CO, back CO; ids in game/skills.h */
    u32 unk_94;                        /* Time Survival timer, frames */
} Player;

typedef char Player_size_check[sizeof(Player) == 0x98 ? 1 : -1];

#endif
