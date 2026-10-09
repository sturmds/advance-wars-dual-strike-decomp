#ifndef GAME_BATTLE_SCENE_H
#define GAME_BATTLE_SCENE_H

#include "types.h"

/* One side of the battle animation in ov002. */
typedef struct BattleSide {
    u8 unk_00;
    u8 unk_01;
    u8 pad_02[6];
    u8 coId; /* low 7 bits: enum CoId */
    u8 pad_09[3];
} BattleSide;

/* The battle animation state that Ov002_gBattleScene points to. */
typedef struct BattleScene {
    u8 unk_00;
    u8 pad_01[3];
    BattleSide sides[2];
} BattleScene;

extern BattleScene *Ov002_gBattleScene;

#endif
