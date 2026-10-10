#ifndef GAME_BATTLE_SCENE_H
#define GAME_BATTLE_SCENE_H

#include "types.h"

/* One side of the battle animation in ov002. */
typedef struct BattleSide {
    u8 hpBefore;                       /* HP before the fight */
    u8 hpAfter;
    u8 terrain;                        /* Combatant.terrain */
    u8 unk_03;                         /* tens of it are drawn as cells by Battle_PutSideCells */
    u8 tileOwner;                      /* owner bits of the unit's tile */
    u8 usesSecondary;                  /* 1: fires its secondary weapon */
    u8 counters;                       /* nonzero: hits back; a guess from Battle_ShowQuote */
    u8 dived;                          /* 1: the unit is dived (MAP_UNIT_DIVED) */
    u8 coId; /* low 7 bits: enum CoId */
    u8 armyStyle;                      /* Player_GetArmyStyle of the owner */
    u8 army;                           /* 1-4: picks the palette at res_battle 0xab4 */
    u8 pad_0b;
} BattleSide;

/* The battle animation state that Ov002_gBattleScene points to. */
typedef struct BattleScene {
    u8 attackerSide;                   /* index into sides */
    u8 weather;                        /* WEATHER_* */
    u8 tileset;                        /* low 7 bits of Unk_02080440_1c4.tileset */
    u8 pad_03;                         /* side 0's unit type, see BATTLE_SIDE_UNIT_TYPE */
    BattleSide sides[2];
} BattleScene;

extern BattleScene *Ov002_gBattleScene;

/* Needed to match: the unit type of a side sits at scene offset 3 + 12 * side,
 * one byte before what BattleScene.sides[side] starts at. */
#define BATTLE_SIDE_UNIT_TYPE(side) (((u8 *)Ov002_gBattleScene)[3 + (side) * sizeof(BattleSide)])

#endif
