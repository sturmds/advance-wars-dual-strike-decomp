#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_action.h"
#include "game/ai_unit_action.h"
#include "game/strike_target.h"
#include "game/keypad.h"
#include "game/player.h"
#include "game/map_unit.h"
#include "game/Unk_02088d04.h"
#include "game/map_header.h"
#include "game/ai/ai_show_action.h"
#include "game/screen_other_map.h"
#include "game/ai.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
/* player index 0-3 of the acting unit: its slot in units divided by 64 */
#define OWNER_INDEX(s) \
  ((((u32)(((s)->actingUnit - (s)->units) & 0xc0)) << 10) >> 16)
#define EXTRA (data_027c027c->unk_5c[data_027c0284])

extern u8 data_0229e19e[];
extern u16 data_021658c8[];
extern int data_022a6dd4;
extern u16 data_0229d8c4;
extern u32 gLinkPlay;

StrikeUnitScoreFunc gStrikeUnitScore;
int data_0229eac8;
StrikeSiteScoreFunc gStrikeSiteScore;
int data_0229eac4;
void* data_0229eacc[2];

void ActingUnit_FinishAction(void);
void BlackBombExplosion_Start(int x, int y, int x2, int y2, int mode);
int MapScript_RunList10(int id, MapUnit* unit);
void func_020edb84(MapUnit* unit);
void Unit_PlayEmergeEffect(int unitType);
void Unit_PlayHideEffect(int unitType);
void* UnitShowTask_CreateMoving(int x, int y, int owner, int unitType,
                                void* arg);
void ActingUnit_UnloadCargo(int cargoSlot);
void UnitType_PlayUnloadSound(int unitType);
int func_020b9914(void);
void UnitShowTask_Destroy(void* task);
void Silo_StartLaunch(int x, int y, int target, int arg, int mode);
void ActingUnit_MoveAndAttackObject(int target, int arg);
void func_020dea20(int target);
int Link_IsActive(void);
void Map_ScrollViewToTile(int arg);
void func_020f5888(int x, int y);
void Player_RefreshUnits(void);
MapUnit* CurrentPlayer_BuyUnit(int x, int y, int unitType);
void Screen_SetCurrentBoth(void* state);
int func_020b9664(void);
void func_020c8888(void);
void UnitAction_ActivateTagPower(void);
void UnitAction_ActivateSuperPower(void);
void UnitAction_ActivateCoPower(void);
void UnitAction_SwapCo(void);
void UnitAction_MoveToOtherMap(void);
void func_020dd89c(void);
void func_020dd9a0(void);
void UnitAction_Supply(void);
void UnitAction_Join(void);
void UnitAction_Repair(int arg);
void UnitAction_Board(void);
void UnitAction_Capture(void);
void UnitShowTask_Create(MapUnit* unit);
void UnpackNibblePairs(u8* packed, u8* nibbles);
void func_020c8290(u8* nibbles);
void Rng_SetState(u32 state);
int Link_GetOwnPlayer(void);

/* Runs one step of showing the action in gUnitAction, picked by
 * gAiTurnState->actionStep. The last steps set the state back to
 * AI_STATE_RUN_QUEUE and actionStep to 0. */
void AiTurn_ShowAction(void) {
  data_0229eac4 = 0;
  switch (gAiTurnState->actionStep) {
    case 0:
      AiShowAction_Step0();
      break;
    case 1:
      AiShowAction_Step1();
      break;
    case 2:
      AiShowAction_Step2();
      break;
    case 3:
      AiShowAction_Step3();
      break;
    case 4:
      AiShowAction_Step4();
      break;
    case 5:
      AiShowAction_Step5();
      break;
    case 6:
      AiShowAction_Step6();
      break;
    case 7:
      AiShowAction_Step7();
      break;
    case 8:
      AiShowAction_Step8();
      break;
    case 9:
      AiShowAction_Step9();
      break;
    case 10:
      AiShowAction_Step10();
      break;
    case 11:
      AiShowAction_Step11();
      break;
  }
}

/* First display step. Restores the RNG state the action was made with,
 * then picks the next step by gUnitAction.action: actions 15 to 18 and 27
 * go straight to step 2, 1, 13, 14 and 19 to 21 do their small update and
 * go to step 2, and every other action makes the saved unit the acting unit
 * with its start and end tiles and goes to step 1. */
void AiShowAction_Step0(void) {
  Rng_SetState(gUnitAction.rngState);
  switch (gUnitAction.action) {
    case 15:
    case 16:
    case 17:
    case 18:
    case 27:
      gAiTurnState->actionStep = 2;
      return;
  }
  if (gUnitAction.action == 1 || gUnitAction.action == 14 ||
      gUnitAction.action == 13 || (u8)(gUnitAction.action + 237) <= 2) {
    if (gUnitAction.action == 19) {
      MAP_STATE->actingUnitIndex = gUnitAction.unitIndex;
      MAP_STATE->actingUnit = &MAP_STATE->units[MAP_STATE->actingUnitIndex];
      MAP_STATE->actingUnit->fuel = gUnitAction.fuelLeft;
    }
    if (gUnitAction.action == 20) {
      MAP_STATE->actingUnitIndex = gUnitAction.unitIndex;
      MAP_STATE->actingUnit = &MAP_STATE->units[MAP_STATE->actingUnitIndex];
      func_020f5888(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y);
    }
    gAiTurnState->actionStep = 2;
    return;
  }
  MAP_STATE->actingUnitIndex = gUnitAction.unitIndex;
  MAP_STATE->actingUnit = &MAP_STATE->units[MAP_STATE->actingUnitIndex];
  MAP_STATE->actingUnitStartX = gUnitAction.fromX;
  MAP_STATE->actingUnitStartY = gUnitAction.fromY;
  MAP_STATE->actingUnitDestX = gUnitAction.x;
  MAP_STATE->actingUnitDestY = gUnitAction.y;
  MAP_STATE->actingUnit->flags &= ~MAP_UNIT_LOCKED;
  if ((MAP_STATE->unk_18 == 16 && gLinkPlay != 0 && Link_GetOwnPlayer() != 0) ||
      MAP_STATE->unk_18 == 21) {
    MAP_STATE->unk_46 = 0;
    MAP_STATE->actingUnit->fuel = gUnitAction.fuelLeft;
    gAiTurnState->actionStep = 1;
  }
  if (func_020b9664() == 0 || data_0229d8c4 != 4) {
    func_020f5888(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y);
  }
  gAiTurnState->actionStep = 1;
}

/* Unpacks the nibbles saved in gUnitAction.packed back into
 * Unk_02080440_48.pathSteps, then moves to step 2. */
void AiShowAction_Step1(void) {
  UnitShowTask_Create(MAP_STATE->actingUnit);
  func_020c8888();
  UnpackNibblePairs(gUnitAction.packed, MAP_STATE->pathSteps);
  func_020c8290(MAP_STATE->pathSteps);
  gAiTurnState->actionStep = 2;
}

/* Second display step: runs the display routine of the action in
 * gUnitAction. Unless the action is 1, 13 to 21 or 27, the acting unit
 * first gets its final tile and is marked as waited. Most actions end by
 * going back to the queue; 4, 5, 8 and 15 and a failed 22 set the next
 * state themselves. Action 27 sets the AI behaviour of a player in both
 * game states. */
void AiShowAction_Step2(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (!(gUnitAction.action == 1 || gUnitAction.action == 14 ||
        gUnitAction.action == 13 || gUnitAction.action == 15 ||
        gUnitAction.action == 16 || gUnitAction.action == 17 ||
        gUnitAction.action == 18 || gUnitAction.action == 19 ||
        gUnitAction.action == 20 || gUnitAction.action == 21 ||
        gUnitAction.action == 27)) {
    MAP_STATE->actingUnit->flags |= MAP_UNIT_WAITED;
    MAP_STATE->actingUnit->x = MAP_STATE->actingUnitDestX;
    MAP_STATE->actingUnit->y = MAP_STATE->actingUnitDestY;
    func_020c8888();
  }
  switch (gUnitAction.action) {
    case 1:
      AiShowAction_BuildUnit();
      break;
    case 14:
      AiShowAction_BuildUnitAlt();
      break;
    case 13:
      AiShowAction_StartTurn();
      break;
    case 15:
      UnitAction_SwapCo();
      if (func_020b9664() == 1) {
        gAiTurnState->state = AI_STATE_RUN_QUEUE;
      } else if (MAP_STATE->players[MAP_STATE->currentPlayer].tagPowerHalf ==
                 TAG_POWER_BACK) {
        gAiTurnState->state = AI_STATE_START;
      } else {
        gAiTurnState->state = 6;
      }
      gAiTurnState->actionStep = 0;
      return;
    case 16:
      UnitAction_ActivateCoPower();
      break;
    case 17:
      UnitAction_ActivateSuperPower();
      break;
    case 18:
      UnitAction_ActivateTagPower();
      break;
    case 2:
      ActingUnit_FinishAction();
      break;
    case 3:
      UnitAction_Capture();
      ActingUnit_FinishAction();
      break;
    case 4:
      AiShowAction_AttackUnit();
      return;
    case 5:
      AiShowAction_AttackObject();
      return;
    case 7:
      UnitAction_Board();
      ActingUnit_FinishAction();
      break;
    case 8:
      AiShowAction_ShowCargoUnit(1);
      AiShowAction_ShowCargoUnit(0);
      gAiTurnState->actionStep = 10;
      return;
    case 10:
      UnitAction_Join();
      ActingUnit_FinishAction();
      break;
    case 6:
      UnitAction_Supply();
      break;
    case 11:
      AiShowAction_Dive();
      ActingUnit_FinishAction();
      break;
    case 12:
      AiShowAction_Emerge();
      ActingUnit_FinishAction();
      break;
    case 20:
      func_020b8230();
      break;
    case 21:
      Ai_FlagCurrentPlayerEnterState18();
      break;
    case 22:
      if (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN &&
          MAP_STATE->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 18) {
        func_020dd9a0();
        ActingUnit_FinishAction();
      } else {
        AiShowAction_Action22();
        return;
      }
      break;
    case 23:
      AiShowAction_BlackBombExplode();
      break;
    case 24:
      UnitAction_Repair(gUnitAction.target - 1);
      ActingUnit_FinishAction();
      break;
    case 25:
      func_020dd89c();
      ActingUnit_FinishAction();
      break;
    case 26:
      ActingUnit_FinishAction();
      UnitAction_MoveToOtherMap();
      break;
    case 27:
      saved0280 = data_027c0280;
      saved027c = data_027c027c;
      Screen_SetCurrentBoth(data_027c0288.unk_00);
      MAP_STATE->players[gUnitAction.target].aiBehaviour = gUnitAction.unk_07;
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      MAP_STATE->players[gUnitAction.target].aiBehaviour = gUnitAction.unk_07;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
  }
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
  gAiTurnState->actionStep = 0;
}

/* Display step of action 1: buys the unit type in gUnitAction.unitIndex at
 * the factory tile (x, y), then jumps to the last display step. */
void AiShowAction_BuildUnit(void) {
  CurrentPlayer_BuyUnit(gUnitAction.x, gUnitAction.y, gUnitAction.unitIndex);
  gAiTurnState->actionStep = 11;
}

/* Display step of action 14: buys the unit type in gUnitAction.unitIndex at
 * the factory tile (x, y), then jumps to the last display step. Same code as action 1; why the AI has a second action is not known. */
void AiShowAction_BuildUnitAlt(void) {
  CurrentPlayer_BuyUnit(gUnitAction.x, gUnitAction.y, gUnitAction.unitIndex);
  gAiTurnState->actionStep = 11;
}

/* Display step of action 13. Guess: a new turn starts. Refreshes the units,
 * restarts the AI turn and copies the next weather and the per-player weather days from
 * the action. */
void AiShowAction_StartTurn(void) {
  int i;

  MAP_STATE->unk_18 = 1;
  Player_RefreshUnits();
  gAiTurnState->state = AI_STATE_START;
  MAP_STATE->unk_1c4->nextWeather = gUnitAction.target;
  EXTRA->sandChance = gUnitAction.unk_07;
  EXTRA->rainChance = gUnitAction.packed[0];
  EXTRA->snowChance = gUnitAction.packed[1];
  for (i = 0; i < PLAYER_SLOT_LAST; i++) {
    EXTRA->weatherDays[i + 1] = gUnitAction.packed[i + 2];
  }
}

/* Display step of action 4 (attack a unit): gUnitAction.target is the
 * defender's slot. Calls func_020f5888 on its tile when unk_2338 is nonzero
 * there, restarts the frame counter and goes to step 7. */
void AiShowAction_AttackUnit(void) {
  int x = MAP_STATE->units[gUnitAction.target].x;
  int y = MAP_STATE->units[gUnitAction.target].y;

  if (MAP_STATE->mapTiles->unk_2338[MAP_STATE->mapTiles->rowStart[y] + x] !=
      0) {
    func_020f5888(x, y);
  }
  data_0229eac8 = 0;
  gAiTurnState->actionStep = 7;
}

/* Display step of action 5 (attack an object): gUnitAction.target and
 * unk_07 are the object's tile. When unk_2338 is nonzero there, calls
 * func_020f5888 on the tile, zeroes the frame counter and goes to step 8;
 * otherwise goes straight to step 4. */
void AiShowAction_AttackObject(void) {
  if (MAP_STATE->mapTiles
          ->unk_2338[MAP_STATE->mapTiles->rowStart[gUnitAction.unk_07] +
                     gUnitAction.target] != 0) {
    func_020f5888(gUnitAction.target, gUnitAction.unk_07);
    data_0229eac8 = 0;
    gAiTurnState->actionStep = 8;
  } else {
    gAiTurnState->actionStep = 4;
  }
}

/* Display step of action 22 when it is not the campaign scripted case (see
 * AiShowAction_Step2): gUnitAction.target and unk_07 are a tile. Like
 * AiShowAction_AttackObject, but the steps are 5 and 9. */
void AiShowAction_Action22(void) {
  int x = gUnitAction.target;
  int y = gUnitAction.unk_07;

  if (MAP_STATE->mapTiles->unk_2338[MAP_STATE->mapTiles->rowStart[y] + x] !=
      0) {
    /* Needed to match: the original narrows the tile to s16 here, where
     * the other callers pass it as it is. */
    func_020f5888((s16)x, (s16)y);
    data_0229eac8 = 0;
    gAiTurnState->actionStep = 9;
  } else {
    gAiTurnState->actionStep = 5;
  }
}

/* Like AiShowAction_Step8, but gUnitAction.target is a unit slot here:
 * the step points Map_ScrollViewToTile at that unit when its tile is nonzero in
 * unk_2338, then moves to step 3 after 30 frames or A. */
void AiShowAction_Step7(void) {
  MapUnit* unit = &MAP_STATE->units[gUnitAction.target];
  int skip = 0;

  if (Link_IsActive() == 0) {
    skip = gKeypad->held & PAD_BUTTON_A;
  }
  if (MAP_STATE->mapTiles
          ->unk_2338[MAP_STATE->mapTiles->rowStart[unit->y] + unit->x] != 0) {
    MAP_STATE->cursorX = unit->x;
    MAP_STATE->cursorY = unit->y;
    Map_ScrollViewToTile(2);
  }
  data_0229eac8++;
  if (data_0229eac8 > 30 || skip) {
    gAiTurnState->actionStep = 3;
  }
}

/* Holds this step for 30 frames, or until A is held when Link_IsActive
 * returns 0, then moves to step 4. data_0229eac8 counts the frames. */
void AiShowAction_Step8(void) {
  int skip = 0;

  if (Link_IsActive() == 0) {
    skip = gKeypad->held & PAD_BUTTON_A;
  }
  MAP_STATE->cursorX = gUnitAction.target;
  MAP_STATE->cursorY = gUnitAction.unk_07;
  Map_ScrollViewToTile(2);
  data_0229eac8++;
  if (data_0229eac8 > 30 || skip) {
    gAiTurnState->actionStep = 4;
  }
}

/* Same wait as AiShowAction_Step8, then moves to step 5. */
void AiShowAction_Step9(void) {
  int skip = 0;

  if (Link_IsActive() == 0) {
    skip = gKeypad->held & PAD_BUTTON_A;
  }
  MAP_STATE->cursorX = gUnitAction.target;
  MAP_STATE->cursorY = gUnitAction.unk_07;
  Map_ScrollViewToTile(2);
  data_0229eac8++;
  if (data_0229eac8 > 30 || skip) {
    gAiTurnState->actionStep = 5;
  }
}

void AiShowAction_Step3(void) {
  func_020dea20(gUnitAction.target);
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
  gAiTurnState->actionStep = 0;
}

void AiShowAction_Step4(void) {
  ActingUnit_MoveAndAttackObject(gUnitAction.target, gUnitAction.unk_07);
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
  gAiTurnState->actionStep = 0;
}

void AiShowAction_Step5(void) {
  ActingUnit_FinishAction();
  Silo_StartLaunch(gUnitAction.x, gUnitAction.y, gUnitAction.target,
                   gUnitAction.unk_07, 3);
  gAiTurnState->actionStep = 6;
}

/* Last display step: back to running the queue. */
void AiShowAction_Step6(void) {
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
  gAiTurnState->actionStep = 0;
}

/* Waits while func_020b9914 returns nonzero, then destroys the two
 * tasks in data_0229eacc, if running, and returns to the queue. */
void AiShowAction_Step10(void) {
  if (func_020b9914() != 0) {
    return;
  }
  if (data_0229eacc[0] != NULL) {
    UnitShowTask_Destroy(data_0229eacc[0]);
  }
  if (data_0229eacc[1] != NULL) {
    UnitShowTask_Destroy(data_0229eacc[1]);
  }
  ActingUnit_FinishAction();
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
  gAiTurnState->actionStep = 0;
}

/* Last display step: back to running the queue. */
void AiShowAction_Step11(void) {
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
  gAiTurnState->actionStep = 0;
}

/* Part of the display of action 8 (unload; Step2 calls it for slot 1, then 0).
 * Starts a task that shows the unit in cargo slot cargoSlot of the acting
 * unit and keeps it in data_0229eacc. The previous value in data_0229e19e
 * picks what the task gets: 0 none, 5 the packed values of the state, else
 * the entry of data_021658c8 for that value. Only the last case also calls
 * ActingUnit_UnloadCargo, counts in data_0229eac4 and calls UnitType_PlayUnloadSound. The
 * meaning of those is not known. */
void AiShowAction_ShowCargoUnit(int cargoSlot) {
  int saved = data_0229e19e[cargoSlot];
  Unk_02080440_48* state;
  MapUnit* cargo;

  data_0229eacc[cargoSlot] = NULL;
  if (saved == 5) {
    state = MAP_STATE;
    cargo = &state->units[state->actingUnit->cargo[cargoSlot]];
    state->pathSteps[0] = 10;
    MAP_STATE->pathSteps[1] = 4; /* needed to match: MAP_STATE is read again */
    state = MAP_STATE;
    data_0229eacc[cargoSlot] = UnitShowTask_CreateMoving(
        (u16)state->actingUnitDestX, (u16)state->actingUnitDestY,
        OWNER_INDEX(state), cargo->type, state->pathSteps);
    return;
  }
  if (saved == 0) {
    return;
  }
  data_022a6dd4 = saved - 1;
  state = MAP_STATE;
  cargo = &state->units[state->actingUnit->cargo[cargoSlot]];
  data_0229eacc[cargoSlot] = UnitShowTask_CreateMoving(
      (u16)state->actingUnitDestX, (u16)state->actingUnitDestY,
      OWNER_INDEX(state), cargo->type, &data_021658c8[saved - 1]);
  ActingUnit_UnloadCargo(cargoSlot);
  data_0229eac4++;
  UnitType_PlayUnloadSound(cargo->type);
}

/* Display step of action 11: the acting unit dives. */
void AiShowAction_Dive(void) {
  Unit_PlayHideEffect(MAP_STATE->actingUnit->type);
  MAP_STATE->actingUnit->flags |= MAP_UNIT_DIVED;
}

/* Display step of action 12: the acting unit ends its dive. */
void AiShowAction_Emerge(void) {
  Unit_PlayEmergeEffect(MAP_STATE->actingUnit->type);
  MAP_STATE->actingUnit->flags &= ~MAP_UNIT_DIVED;
}

/* Display step of unit action 20 (see AiShowAction_Step2): tries
 * MapScript_RunList10 with 47 and the acting unit, and falls back to
 * func_020edb84 when that returns 0. */
void func_020b8230(void) {
  if (MapScript_RunList10(47, MAP_STATE->actingUnit)) {
    return;
  }
  func_020edb84(MAP_STATE->actingUnit);
}

/* Display step of unit action 23, the Black Bomb explosion: the acting
 * unit's hp goes to 0, then BlackBombExplosion_Start draws the effect at the action's
 * tile. */
void AiShowAction_BlackBombExplode(void) {
  MAP_STATE->actingUnit->hp = 0;
  ActingUnit_FinishAction();
  BlackBombExplosion_Start(gUnitAction.x, gUnitAction.y, gUnitAction.x,
                           gUnitAction.y, 3);
}
