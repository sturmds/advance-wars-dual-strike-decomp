#ifndef GAME_BATTLE_SCENE_H
#define GAME_BATTLE_SCENE_H

#include "types.h"

/* One side of the battle animation in ov002. */
typedef struct BattleSide {
    u8 hpBefore;                       /* HP before the fight */
    u8 hpAfter;
    u8 pad_02[4];
    u8 counters;                       /* nonzero: hits back; a guess from Battle_ShowQuote */
    u8 pad_07;
    u8 coId; /* low 7 bits: enum CoId */
    u8 pad_09;
    u8 army;                           /* 1-4: picks the palette at res_battle 0xab4 */
    u8 pad_0b;
} BattleSide;

/* The battle animation state that Ov002_gBattleScene points to. */
typedef struct BattleScene {
    u8 attackerSide;                   /* index into sides */
    u8 pad_01[3];
    BattleSide sides[2];
} BattleScene;

extern BattleScene *Ov002_gBattleScene;

/* Needed to match: the unit type of a side sits at scene offset 3 + 12 * side,
 * one byte before what BattleScene.sides[side] starts at. */
#define BATTLE_SIDE_UNIT_TYPE(side) (((u8 *)Ov002_gBattleScene)[3 + (side) * sizeof(BattleSide)])

#endif
