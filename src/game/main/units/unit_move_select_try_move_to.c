#include "types.h"
#include "game/Unk_02080440.h"
#include "game/move_path.h"
#include "game/play_records.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern PlayRecords gPlayRecords;

BOOL ActingUnit_CanMoveTo(int x, int y);
int UnitPath_StopAtEnemy(int startX, int startY, int unitType, s8* path,
                         int end);
int UnitPath_CountSteps(const s8* path);
void MovePath_ToSteps(void);
void Sound_PlaySe(int se);
void func_020b9d1c(void);
void func_020c8290(u8* nibbles);
void func_020f77bc(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

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
  GAME->actingUnitStartX = GAME->actingUnit->x;
  GAME->actingUnitStartY = GAME->actingUnit->y;
  GAME->actingUnitDestX = x;
  GAME->actingUnitDestY = y;
  func_020b9d1c();
  GAME->unk_1c = 3;
  if (GAME->actingUnitStartX == GAME->actingUnitDestX &&
      GAME->actingUnitStartY == GAME->actingUnitDestY) {
    return;
  }
  MovePath_ToSteps();
  if (UnitPath_StopAtEnemy(GAME->actingUnitStartX, GAME->actingUnitStartY,
                           GAME->actingUnit->type, (s8*)GAME->pathSteps,
                           PATH_STEP_END) == 1) {
    GAME->unk_1c = 5;
  }
  func_020c8290(GAME->pathSteps);
  if (GAME->players[GAME->currentPlayer].controller !=
      PLAYER_CONTROLLER_HUMAN) {
    return;
  }
  if (gPlayRecords.tilesMoved + UnitPath_CountSteps((s8*)GAME->pathSteps) >=
      999999) {
    gPlayRecords.tilesMoved = 999999;
  } else {
    gPlayRecords.tilesMoved += UnitPath_CountSteps((s8*)GAME->pathSteps);
  }
}
