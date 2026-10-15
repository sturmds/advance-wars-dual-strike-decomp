#include "types.h"
#include "game/Unk_02080440.h"
#include "game/move_path.h"
#include "game/play_records.h"
#include "game/player.h"

BOOL ActingUnit_CanMoveTo(int x, int y);
int UnitPath_StopAtEnemy(int startX, int startY, int unitType, s8* path,
                         int end);
int UnitPath_CountSteps(const s8* path);
void MovePath_ToSteps(void);
void Sound_PlaySe(int se);
void func_020b9d1c(void);
void func_020c8290(u8* nibbles);
void func_020f77bc(void);

/* Picks (x, y) as the acting unit's destination. Plays the refusal sound
 * when it cannot move there. Otherwise stores the start and end tiles,
 * turns the drawn path into steps, cuts them at a blocking enemy unit
 * (unk_1c 5 instead of 3), and adds the steps to the tiles-moved record for
 * a human player. */
void UnitMoveSelect_TryMoveTo(int x, int y) {
  if (!ActingUnit_CanMoveTo(x, y)) {
    Sound_PlaySe(0x10008);
    return;
  }
  func_020f77bc();
  MAP_STATE->actingUnitStartX = MAP_STATE->actingUnit->x;
  MAP_STATE->actingUnitStartY = MAP_STATE->actingUnit->y;
  MAP_STATE->actingUnitDestX = x;
  MAP_STATE->actingUnitDestY = y;
  func_020b9d1c();
  MAP_STATE->unk_1c = 3;
  if (MAP_STATE->actingUnitStartX == MAP_STATE->actingUnitDestX &&
      MAP_STATE->actingUnitStartY == MAP_STATE->actingUnitDestY) {
    return;
  }
  MovePath_ToSteps();
  if (UnitPath_StopAtEnemy(MAP_STATE->actingUnitStartX,
                           MAP_STATE->actingUnitStartY,
                           MAP_STATE->actingUnit->type,
                           (s8*)MAP_STATE->pathSteps, PATH_STEP_END) == 1) {
    MAP_STATE->unk_1c = 5;
  }
  func_020c8290(MAP_STATE->pathSteps);
  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller !=
      PLAYER_CONTROLLER_HUMAN) {
    return;
  }
  if (gPlayRecords.tilesMoved +
          UnitPath_CountSteps((s8*)MAP_STATE->pathSteps) >=
      999999) {
    gPlayRecords.tilesMoved = 999999;
  } else {
    gPlayRecords.tilesMoved += UnitPath_CountSteps((s8*)MAP_STATE->pathSteps);
  }
}
