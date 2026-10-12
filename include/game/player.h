#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "types.h"

/* CO id bits of PlayerCo.id and of other CO bytes. */
#define PLAYER_CO_ID_MASK 0x7f

/* Player.front and back: CO id, power state, power meter. */
typedef struct PlayerCo {
  u32 id : 8; /* enum CoId in bits 0-6; mask with PLAYER_CO_ID_MASK */
  u32 powerReady : 2;
  u32 powerState : 2; /* enum CoPowerState */
  u32 unk_12 : 2;
  u32 powerUses : 4; /* stops at 10 */
  u32 unk_18 : 3;
  u32 charge : 11; /* power meter */
} PlayerCo;

/* Values of Player.controller. */
#define PLAYER_CONTROLLER_NONE 0
#define PLAYER_CONTROLLER_HUMAN 1
#define PLAYER_CONTROLLER_AI 2

/* Army colour, the palette (Player.armyColour, MapHeader.army): 1 Orange
 * Star (red) ... 5 Black Hole (dark purple). Army style, the sprite set
 * (Player.armyStyle), uses the same numbers. */
#define ARMY_BLACK_HOLE 5

/* Player.tagPowerHalf: which CO's half of a tag power runs (doc 890). */
#define TAG_POWER_FRONT 1
#define TAG_POWER_BACK 2

/* Player.aiBehaviour. Names from doc 890. */
#define AI_BEHAVIOUR_STRIKE 0
#define AI_BEHAVIOUR_ASSAULT 1
#define AI_BEHAVIOUR_GENERAL 2
#define AI_BEHAVIOUR_DEFENSE 3

/* Index into Player.destroyedCounts: Oozium destroyed, up to 99 (Combat_ApplyResult). */
#define PLAYER_DESTROYED_OOZIUM 3

/* One player slot, 0x98 bytes, in the array at Unk_02080440_48.players:
 * 5 slots, slot 0 neutral, then players 1-4. Layout from romhacking.net doc 890 (RadioTails);
 * fields named here have been seen in code, unk_ fields carry the
 * document's reading as a comment. */
typedef struct Player {
  s32 funds;
  u32 unk_04;       /* money spent */
  u32 unk_08;       /* income */
  u32 damageTaken;  /* damage taken, in funds (Unit_AddDamageTaken) */
  u8 unk_10[4];     /* bases, cities, airports, ports owned */
  u8 comTowers;     /* Com Towers owned */
  u8 propertyCount; /* properties owned (func_020c4448 counts them) */
  u16 defeated;     /* nonzero once the player is out (Player_IsInGame) */
  u8 defeatCode; /* why the player went out (LinkPlayer_MarkDefeated); cleared by Players_Reset */
  u8 armyColour; /* palette: 1 Orange Star ... 5 Black Hole; set from MapHeader.army at battle start (Players_ApplyBattleSetup) */
  u8 controller; /* PLAYER_CONTROLLER_* */
  u8 visionGrids; /* vision grids its sight goes into, bit n for grid visionGrid<n> (game/vision.h); Players_AssignVisionGrids sets it each turn */
  u16 turnKills;  /* kills this turn */
  u16 mostTurnKills; /* most kills in one turn */
  u8 destroyedCounts
      [5]; /* things this player destroyed, by kind, up to 99 each; index 3: Oozium (PLAYER_DESTROYED_OOZIUM) */
  u8 tagPowerHalf; /* TAG_POWER_FRONT or TAG_POWER_BACK while a tag power is on */
  s16 firepowerBonus; /* set to 10 while a power is on */
  s16 defenseBonus;   /* set to 10 while a power is on */
  s8 terrainDefense; /* added to terrain defense; enemy Sonja-style star cuts make it negative */
  u8 moveChart; /* enum MoveChart: index into CoPowerBlock.movementCost */
  u8 team;
  u8 armyStyle; /* sprite set of the front CO's army: CoRecord.armyStyle + 1 (Players_ApplyBattleSetup, func_020fed58); see Player_GetArmyStyle */
  u8 aiBehaviour; /* AI_BEHAVIOUR_* */
  u8 hqX; /* HQ tile x in bits 0-6; bit 7: no HQ. ORed with MapHeader.slotFlags[slot].unk_0 at map start */
  u8 hqY; /* HQ tile y in bits 0-6. ORed with MapHeader.slotFlags[slot].unk_1 */
  u8 savedCursorX; /* map cursor tile saved by func_020ddb38 outside link play; CurrentPlayer_StartTurn puts the cursor back there */
  u8 savedCursorY;
  u8 unk_33; /* cleared by Players_Reset */
  u8 unk_34;
  u8 unitCount; /* units on the map: +1 per unit built, up to 255 (Player_IncUnitCount), -1 per unit lost (Player_DecUnitCount) */
  u8 unitsBuilt[27]; /* per unit type, up to 255; see Player_CountUnitBuilt */
  u8 unitsLost[27];  /* per unit type (doc 890) */
  PlayerCo front;
  PlayerCo back;
  u32 skills[2][4];   /* front CO, back CO; ids in game/skills.h */
  u32 playTimeFrames; /* frames spent on this player's turns (Game_TickPlayerTimer); Time Survival's timer */
} Player;

typedef char Player_size_check[sizeof(Player) == 0x98 ? 1 : -1];

#endif
