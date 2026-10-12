#ifndef GAME_AI_UNIT_ACTION_H
#define GAME_AI_UNIT_ACTION_H

#include "types.h"

/* State of the AI turn, gAiTurnState->state. AiTurn_Update
 * switches on it. */
#define AI_STATE_START 0     /* set up the turn */
#define AI_STATE_RUN_PHASE 1 /* run the unit action step of the current phase */
#define AI_STATE_RUN_QUEUE 2 /* call gAiQueueRoutine for each queue entry */
#define AI_STATE_SHOW_ACTION 3 /* a queued unit acted; name is a guess */
#define AI_STATE_END_ACTIONS 4 /* unit actions done */

/* Actions passed to AiUnit_PerformAction(x, y, action, arg3, arg4). */
#define AI_ACTION_WAIT 2
#define AI_ACTION_CAPTURE 3
#define AI_ACTION_ATTACK_UNIT 4 /* arg3: target unit slot */
#define AI_ACTION_ATTACK_TILE 5 /* arg3, arg4: x, y */
#define AI_ACTION_SUPPLY 6      /* name is a guess */
#define AI_ACTION_BOARD \
  7 /* move onto a transport (Lander, Black Boat, Cruiser, Carrier) */
#define AI_ACTION_UNLOAD \
  8 /* arg3, arg4: direction of each cargo unit, 1 up, 2 right, 3 down, 4 left */
#define AI_ACTION_JOIN 10
#define AI_ACTION_HIDE 11
#define AI_ACTION_APPEAR 12
#define AI_ACTION_LAUNCH_SILO 22 /* arg3, arg4: target x, y */
#define AI_ACTION_REPAIR \
  24 /* arg3: direction of the repaired unit; name is a guess */
#define AI_ACTION_OOZIUM_EAT 25 /* move onto the enemy unit at (x, y) */

/* Queue entry that is not a unit index: the step runs once. */
#define AI_QUEUE_RUN_ONCE 0x40

/* gAiTurnState->flags, as what an indirect unit may shoot besides units. */
typedef struct AiTargetFlags {
  u8 threatMapsBuilt
      : 1; /* set by Ai_BuildThreatMaps, cleared for each queued unit */
  u8 unk_1 : 1;
  u8 attackTiles : 1;   /* tiles with hp (Unk_02080440_48.tileHp) */
  u8 attackObjects : 1; /* MapObject */
  u8 unk_4 : 4;
} AiTargetFlags;

typedef struct AiTurnState {
  u8 state;          /* AI_STATE_* */
  u8 actionStep;     /* step of AiTurn_ShowAction */
  u8 phase;          /* index into the unit phase tables */
  u8 roll;           /* 0-99, rolled for each queued unit */
  u8 captureTargets; /* set by AiTurn_CountCaptureTargets: sum of Player_CountPropertiesPlusOne for neutral and every enemy player in the game */
  u8 unk_05;
  u8 propertyRegions; /* set by AiTurn_MarkReachableTiles */
  u8 flags; /* bits 0-1: cleared for each queued unit; 2, 3: see AiTurn_Start and AiTargetFlags */
  u8 stepFlags; /* AI_STEP_*; cleared for each queued unit */
  u8 pad_09;
  u16 domainsAny
      : 3; /* AI_DOMAIN_* bits, set by AiTurn_FindUnitDomains: every player and tile */
  u16 domainsOwn : 3; /* the same for the current player's units and tiles */
  u16 domainsEnemy
      : 3; /* AI_DOMAIN_* bits, set by AiTurn_FindUnitDomains: players on other teams, and tiles the current player's team does not own */
  u16 factoryDomainsOwn
      : 3; /* copy of domainsOwn before the units are counted */
  u16 factoryDomainsEnemy
      : 3; /* copy of domainsEnemy before the units are counted */
} AiTurnState;

/* Bits in the AiTurnState domain fields: ground (a base, or a unit that is not a ship or aircraft),
 * air (airport, aircraft), sea (port, ship). */
#define AI_DOMAIN_GROUND 1
#define AI_DOMAIN_AIR 2
#define AI_DOMAIN_SEA 4

/* AiTurnState.stepFlags bits. Names of bits without a decompiled setter
 * or reader describe what the known side does. */
#define AI_STEP_UNIT_AT_TARGET \
  0x01 /* AiUnit_HandleUnitAtTarget found an own unit on the target */
#define AI_STEP_STAY \
  0x02 /* with AI_STEP_UNIT_AT_TARGET, AiUnit_MoveToSafestTile does nothing */
#define AI_STEP_IGNORE_TERRAIN \
  0x10 /* AiUnit_MoveToSafestTile ignores terrain stars */
#define AI_STEP_UNLOAD_SCORED \
  0x08 /* AiSeaTransport_ScoreUnloadTiles scored a tile; the sea transport routines then run func_020b5bf4 */
#define AI_STEP_ATTACK_DONE \
  0x20 /* guess: set after the attack steps of a queue routine */
#define AI_STEP_SWAP_CARGO \
  0x40 /* AiUnit_AdjustAction swaps the two cargo slots once */
#define AI_STEP_NO_ADJUST 0x80 /* AiUnit_AdjustAction leaves the action alone */

/* The unit action steps (pointer tables at 0x02161a68 and 0x02161ae8)
 * fill a queue of unit indices, ended by 0, and pick a routine to run
 * on each entry. */
extern AiTurnState* gAiTurnState;

#define AI_TARGET_FLAGS ((AiTargetFlags*)&gAiTurnState->flags)
extern u8* gAiQueueCursor;            /* write position in gAiQueue */
extern void (*gAiQueueRoutine)(void); /* run on each entry */
extern u8 gAiQueue[];
extern u8 gAiQueueSortKeys[]; /* sort key of each gAiQueue entry, same index */

/* Each sea transport routine sets the test and the score that
 * AiSeaTransport_ScoreUnloadTiles uses on the tiles it reaches. */
extern BOOL (*gAiUnloadTest)(void);
extern int (*gAiUnloadScore)(void);
void AiSeaTransport_ScoreUnloadTiles(void);

/* AiAttack_Score flags. The low 16 bits hold the defender's unit slot. */
#define AI_ATTACK_DEFENDER_MASK 0xffff
#define AI_ATTACK_HEAVY_HIT \
  0x10000 /* target armed, and killed or loses 30 hp or more */
#define AI_ATTACK_SAFE 0x20000 /* attacker survives and loses under 10 hp */
#define AI_ATTACK_TANK_VALUE \
  0x40000 /* target armed, damage worth 60% of a Tank or more */
#define AI_ATTACK_COST_VALUE \
  0x80000 /* vehicles other than Recon: see AiAttack_Score */
#define AI_ATTACK_NO_CHIP \
  0x100000 /* aiBehaviour not AI_BEHAVIOUR_STRIKE, target not a Sub: target killed or loses 20 hp or more, and not an unarmed ship */

#endif
