#include "types.h"

void func_020c4024(void);
void func_020c4028(void);
void MapView_ScrollStep(int step);
void Map_ScrollViewToTile(int mode);
void MovePath_Draw(void);
BOOL MapView_IsAtRest(void);
BOOL UnitMoveSelect_HandleButtons(void);
void UnitMoveSelect_HandleTouch(void);

/* One frame of game state 15, sub-state 1 (unk_1c): the player picks where
 * the acting unit moves. Moves the cursor and view and draws the move path;
 * once the view stops scrolling, A and B go first and touch is read only
 * when neither was pressed. */
void UnitMoveSelect_Update(void) {
  func_020c4024();
  func_020c4028();
  MapView_ScrollStep(4);
  Map_ScrollViewToTile(1);
  MovePath_Draw();
  if (!MapView_IsAtRest()) {
    return;
  }
  if (UnitMoveSelect_HandleButtons()) {
    return;
  }
  UnitMoveSelect_HandleTouch();
}
