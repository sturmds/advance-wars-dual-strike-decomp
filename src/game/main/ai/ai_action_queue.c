#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/ai/ai_unit_act.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/terrain.h"
#include "game/ai/ai_action_queue.h"
#include "game/ai/ai_turn_setup.h"
#include "game/ai/ai_turn_marks.h"
#include "game/ai/ai_power.h"
#include "game/ai/ai_unit_claims.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
/* UnitStats.aiClass value that no unit type has in retail. */
#define UNIT_AI_CLASS_3 3
/* Slots 1-50 of the current player's 64; slot 0 would read as the end of
 * the queue for player 1. */
#define FIRST_UNIT (Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1)

BOOL CanUseSuperPower(int player, int slot);
/* Decides whether the AI uses a CO Power or Super CO Power (calls SuperPowerCost). */
/* TRUE on a missile silo; otherwise depends on the tile (Map_IsCapturableByActingUnit). */
BOOL Map_IsSiloOrCapturableTile(int x, int y);
/* Vision of a unit type for a player, after CO bonuses and rain. */
int Player_GetUnitVision(int player, int unitType);
int GetUnitMaxRange(int player, int unitType);
int GetUnitMovement(int player, int unitType);

/* Bubble sort of the first count entries of gAiQueue by their keys in
 * gAiQueueSortKeys, largest key first. */
void AiQueue_SortDescending(int count) {
  int i;
  int j;
  u8 tmp;

  if (count < 2) {
    return;
  }
  for (i = 0; i <= count - 2; i++) {
    for (j = count - 2; j >= i; j--) {
      if (gAiQueueSortKeys[j] < gAiQueueSortKeys[j + 1]) {
        tmp = gAiQueueSortKeys[j];
        gAiQueueSortKeys[j] = gAiQueueSortKeys[j + 1];
        gAiQueueSortKeys[j + 1] = tmp;
        tmp = gAiQueue[j];
        gAiQueue[j] = gAiQueue[j + 1];
        gAiQueue[j + 1] = tmp;
      }
    }
  }
}

/* Bubble sort of the first count entries of gAiQueue by their keys in
 * gAiQueueSortKeys, smallest key first. */
void AiQueue_SortAscending(int count) {
  int i;
  int j;
  u8 tmp;

  if (count < 2) {
    return;
  }
  for (i = 0; i <= count - 2; i++) {
    for (j = count - 2; j >= i; j--) {
      if (gAiQueueSortKeys[j] > gAiQueueSortKeys[j + 1]) {
        tmp = gAiQueueSortKeys[j];
        gAiQueueSortKeys[j] = gAiQueueSortKeys[j + 1];
        gAiQueueSortKeys[j + 1] = tmp;
        tmp = gAiQueue[j];
        gAiQueue[j] = gAiQueue[j + 1];
        gAiQueue[j + 1] = tmp;
      }
    }
  }
}

/* Sorts gAiQueue by hp, highest first. A unit that already has a target
 * ranks one above a unit with the same hp and none. */
void AiQueue_SortByHp(void) {
  int i;
  MapUnit* unit;

  /* needed to match: a plain for condition rotates the loop */
  for (i = 0;; i++) {
    if (gAiQueue[i] == 0) {
      break;
    }
    unit = &MAP_STATE->units[gAiQueue[i]];
    gAiQueueSortKeys[i] = unit->hp * 2;
    if (unit->aiTargetX != MAP_UNIT_NO_TARGET) {
      gAiQueueSortKeys[i]++;
    }
  }
  AiQueue_SortDescending(i);
}

/* Sorts gAiQueue by each unit's movement; smallest first unless
 * descending. */
void AiQueue_SortByMovement(BOOL descending) {
  int i;
  int type;

  /* needed to match: a plain for condition rotates the loop */
  for (i = 0;; i++) {
    if (gAiQueue[i] == 0) {
      break;
    }
    type = MAP_STATE->units[gAiQueue[i]].type;
    if (type == UNIT_BOMBER) {
      /* a Bomber counts as one more than a Fighter */
      gAiQueueSortKeys[i] =
          GetUnitMovement(MAP_STATE->currentPlayer, UNIT_FIGHTER) + 1;
    } else {
      gAiQueueSortKeys[i] = GetUnitMovement(MAP_STATE->currentPlayer, type);
    }
  }
  if (!descending) {
    AiQueue_SortAscending(i);
  } else {
    AiQueue_SortDescending(i);
  }
}

/* Sorts gAiQueue by each unit's maximum attack range, smallest first. */
void AiQueue_SortByMaxRange(void) {
  int i;

  /* needed to match: a plain for condition rotates the loop */
  for (i = 0;; i++) {
    if (gAiQueue[i] == 0) {
      break;
    }
    gAiQueueSortKeys[i] = GetUnitMaxRange(MAP_STATE->currentPlayer,
                                          MAP_STATE->units[gAiQueue[i]].type);
  }
  AiQueue_SortAscending(i);
}

/* Sorts gAiQueue by each unit's vision; smallest first unless
 * descending. */
void AiQueue_SortByVision(BOOL descending) {
  int i;

  /* needed to match: a plain for condition rotates the loop */
  for (i = 0;; i++) {
    if (gAiQueue[i] == 0) {
      break;
    }
    gAiQueueSortKeys[i] = Player_GetUnitVision(
        MAP_STATE->currentPlayer, MAP_STATE->units[gAiQueue[i]].type);
  }
  if (!descending) {
    AiQueue_SortAscending(i);
  } else {
    AiQueue_SortDescending(i);
  }
}

/* Sorts gAiQueue by free cargo slots (0-2), most first. */
void AiQueue_SortBlackBoats(void) {
  int i;
  MapUnit* unit;

  /* needed to match: a plain for condition rotates the loop */
  for (i = 0;; i++) {
    if (gAiQueue[i] == 0) {
      break;
    }
    unit = &MAP_STATE->units[gAiQueue[i]];
    if (unit->cargo[0] == 0) {
      gAiQueueSortKeys[i] = 2;
    } else if (unit->cargo[1] == 0) {
      gAiQueueSortKeys[i] = 1;
    } else {
      gAiQueueSortKeys[i] = 0;
    }
  }
  AiQueue_SortDescending(i);
}

/* Sorts gAiQueue, largest key first. Units on a port or shoal come
 * before the rest; in each group, empty transports come first, then
 * full ones, then those with one unit aboard. */
void AiQueue_SortSeaTransports(void) {
  int i;
  MapUnit* unit;
  MapTiles* map;
  int terrain;

  /* needed to match: a plain for condition rotates the loop */
  for (i = 0;; i++) {
    if (gAiQueue[i] == 0) {
      break;
    }
    unit = &MAP_STATE->units[gAiQueue[i]];
    map = MAP_STATE->mapTiles;
    terrain = MAP_TILE_TERRAIN(map->tiles[map->rowStart[unit->y] + unit->x]);
    if (terrain == TERRAIN_PORT || terrain == TERRAIN_SHOAL) {
      if (unit->cargo[0] == 0) {
        gAiQueueSortKeys[i] = 5;
      } else if (unit->cargo[1] != 0) {
        gAiQueueSortKeys[i] = 4;
      } else {
        gAiQueueSortKeys[i] = 3;
      }
    } else {
      if (unit->cargo[0] == 0) {
        gAiQueueSortKeys[i] = 2;
      } else if (unit->cargo[1] != 0) {
        gAiQueueSortKeys[i] = 1;
      } else {
        gAiQueueSortKeys[i] = 0;
      }
    }
  }
  AiQueue_SortDescending(i);
}

/* Queues the current player's infantry and mechs that have not acted
 * and are partway through a capture. Only slots 1-50 of the player are
 * checked. */
void AiAction_QueueCapturingUnits(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_FOOT &&
        unit->captureProgress != 0) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiFoot_ContinueCapture;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Queues the current player's infantry and mechs that have not acted,
 * whatever their AI order, highest hp first. Only slots 1-50 of the
 * player are checked. Same queue as AiAction_QueueFootUnits2 with a
 * different action routine. */
void AiAction_QueueFootUnits(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_FOOT) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  AiQueue_SortByHp();
  gAiQueueRoutine = AiFoot_MoveAndCapture;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Queues the current player's infantry and mechs that have not acted,
 * whatever their AI order, highest hp first. Only slots 1-50 of the
 * player are checked. Same queue as AiAction_QueueFootUnits with a
 * different action routine. */
void AiAction_QueueFootUnits2(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_FOOT) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  AiQueue_SortByHp();
  gAiQueueRoutine = AiFoot_MoveAndAttack;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Queues every direct combat unit of the current player that has not
 * acted, whatever its AI order. Only slots 1-50 of the player are
 * checked. Only the second step table (0x02161ae8, the Fog of War
 * list) uses this step, and it sorts by vision with the largest first. */
void AiAction_QueueAllDirectUnitsFog(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_DIRECT) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiQueue_SortByVision(TRUE);
  } else {
    AiQueue_SortByMovement(FALSE);
  }
  gAiQueueRoutine = AiUnit_ActByOrderFog;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Queues every direct combat unit of the current player that has not
 * acted, whatever its AI order. Only slots 1-50 of the player are
 * checked. */
void AiAction_QueueAllDirectUnits(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_DIRECT) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiQueue_SortByVision(FALSE);
  } else {
    AiQueue_SortByMovement(FALSE);
  }
  gAiQueueRoutine = AiUnit_ActByOrder;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Queues the current player's direct combat units that have not acted,
 * leaving out units with AI order 4 unless Map_IsSiloOrCapturableTile accepts their
 * tile (AiAction_QueueDirectUnitsOrder4 takes those later). Only slots
 * 1-50 of the player are checked. */
void AiAction_QueueDirectUnits(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_DIRECT &&
        (Map_IsSiloOrCapturableTile(unit->x, unit->y) || unit->aiOrder != 4)) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiQueue_SortByVision(FALSE);
  } else {
    AiQueue_SortByMovement(FALSE);
  }
  gAiQueueRoutine = AiUnit_ActByOrder;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Queues the current player's direct combat units that have not acted
 * and have AI order 4. Only slots 1-50 of the player are checked. */
void AiAction_QueueDirectUnitsOrder4(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_DIRECT &&
        unit->aiOrder == 4) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiQueue_SortByVision(FALSE);
  } else {
    AiQueue_SortByMovement(FALSE);
  }
  gAiQueueRoutine = AiUnit_ActByOrder;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Queues the current player's indirect units (AI_CLASS_INDIRECT) that
 * have not waited yet, sorts them by range (AiQueue_SortByMaxRange), and
 * runs AiUnit_ActByOrderGroup on each. */
void AiAction_QueueIndirectUnits(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_INDIRECT) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  AiQueue_SortByMaxRange();
  gAiQueueRoutine = AiUnit_ActByOrderGroup;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Queues the current player's indirect units (AI_CLASS_INDIRECT) that
 * have not waited yet, hands the queue to AiQueue_SortByMovement (argument 1),
 * and runs AiIndirect_MoveByOrder on each. */
void AiAction_QueueIndirectMoves(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_INDIRECT) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  AiQueue_SortByMovement(1);
  gAiQueueRoutine = AiIndirect_MoveByOrder;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

void func_0208e8fc(void) {
  gAiQueueCursor = gAiQueue;
  *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiTurn_BuildCaptureMap;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

void func_0208e89c(void) {
  gAiQueueCursor = gAiQueue;
  *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = Ai_ClearTransportsSentToUnit;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

void AiAction_QueueCoPowerCheck(void) {
  gAiQueueCursor = gAiQueue;
  *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiPlayer_TryUsePower;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

void func_0208e7dc(void) {
  gAiQueueCursor = gAiQueue;
  *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiTurn_ClaimCaptureTargets;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

void func_0208e77c(void) {
  gAiQueueCursor = gAiQueue;
  *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiTurn_ReleaseCaptureClaims;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Queues the current player's APCs and T-Copters that carry nothing and
 * have not waited yet, hands the queue to AiQueue_SortByMovement, and runs
 * AiEmptyTransport_Act on each. */
void AiAction_QueueEmptyTransports(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_TRANSPORT &&
        unit->cargo[0] == 0) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  AiQueue_SortByMovement(0);
  gAiQueueRoutine = AiEmptyTransport_Act;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Queues the current player's APCs and T-Copters that carry a unit and
 * have not waited yet, hands the queue to AiQueue_SortByMovement, and runs
 * AiTransport_Unload on each. Picks the same units as
 * AiAction_QueueLoadedTransports; only the routine differs. */
void AiAction_QueueTransportUnloads(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_TRANSPORT &&
        unit->cargo[0] != 0) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  AiQueue_SortByMovement(0);
  gAiQueueRoutine = AiTransport_Unload;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Queues the current player's APCs and T-Copters that carry a unit and
 * have not waited yet, hands the queue to AiQueue_SortByMovement, and runs
 * AiLoadedTransport_Act on each. */
void AiAction_QueueLoadedTransports(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_TRANSPORT &&
        unit->cargo[0] != 0) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  AiQueue_SortByMovement(0);
  gAiQueueRoutine = AiLoadedTransport_Act;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Queues the current player's Black Boats and Landers that have not
 * waited yet, hands the queue to AiQueue_SortSeaTransports, and runs
 * AiSeaTransport_Act on each. */
void AiAction_QueueSeaTransports(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_SEA_TRANSPORT) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  AiQueue_SortSeaTransports();
  gAiQueueRoutine = AiSeaTransport_Act;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* run on each queued unit */
/* Queues the current player's units that have not acted yet and whose
 * UnitStats.aiClass is 3. No unit type has that value, so in retail the
 * queue stays empty. The scan skips the player's first unit slot and
 * covers the next 50. */
void AiAction_QueueClass3Units(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == UNIT_AI_CLASS_3) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiClass3_Act;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* run on each queued Black Boat */
/* Queues the current player's Black Boats that have not acted yet. The
 * scan skips the player's first unit slot and covers the next 50. */
void AiAction_QueueBlackBoats(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        unit->type == UNIT_BLACK_BOAT) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  AiQueue_SortBlackBoats();
  gAiQueueRoutine = AiBlackBoat_Act;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* run on each queued Black Bomb; explodes it, per document 890 */
/* Queues the current player's Black Bombs that have not acted yet. The scan
 * skips the player's first unit slot and covers the next 50. */
void AiAction_QueueBlackBombExplosions(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        unit->type == UNIT_BLACK_BOMB) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiBlackBomb_Explode;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* run on each queued Black Bomb */
/* Queues the current player's Black Bombs that have not acted yet. The scan
 * skips the player's first unit slot and covers the next 50. */
void AiAction_QueueBlackBombs(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        unit->type == UNIT_BLACK_BOMB) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiBlackBomb_Act;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* run on each queued Black Bomb */
/* Same as AiAction_QueueBlackBombs, but only when the front CO is Eagle
 * and can use the Super CO Power; otherwise the queue stays empty. The
 * table runs this step earlier in the turn than AiAction_QueueBlackBombs. */
void AiAction_QueueBlackBombsForEagle(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  if ((MAP_STATE->players[MAP_STATE->currentPlayer].front.id &
       PLAYER_CO_ID_MASK) == CO_EAGLE &&
      CanUseSuperPower(MAP_STATE->currentPlayer, 0)) {
    for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
         i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
      unit = &MAP_STATE->units[i];
      if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
          unit->type == UNIT_BLACK_BOMB) {
        *gAiQueueCursor++ = i;
      }
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiBlackBomb_Act;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* run on each queued Oozium */
/* Queues the current player's Oozium that have not acted yet. The scan
 * skips the player's first unit slot and covers the next 50. */
void AiAction_QueueOoziums(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        unit->type == UNIT_OOZIUM) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiOozium_Act;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}

/* Last entry of both AI unit action tables. */
void AiAction_EndUnitActions(void) {
  gAiTurnState->state = AI_STATE_END_ACTIONS;
}
