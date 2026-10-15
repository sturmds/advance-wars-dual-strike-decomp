#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/move_path.h"
#include "game/icon.h"

extern u8 data_021492c0[];
extern MovePath* gMovePath;

void TransferQueue_Add(const void* src, void* dst, u32 size);
int Unit_GetMovePoints(MapUnit* unit);
void MovePath_Truncate(int count);
void MovePath_AddTile(s8 x, s8 y);
void MovePath_SetCursor(int a, int b);
void MovePath_FollowCursor(void);

/* Starts a move path for the acting unit: loads the path graphics unless
 * data_027c027c and data_027c0288 hold the same pointer, empties the path,
 * puts the unit's own tile first with its full move points, and runs
 * MovePath_FollowCursor. */
void MovePath_Start(void) {
  if (data_027c027c != data_027c0288.unk_00) {
    TransferQueue_Add(data_021492c0, data_027c0280->objVram + 0x3280, 0xa00);
    IconBank_LoadPalette(1, 19);
  }
  gMovePath->startMoves = Unit_GetMovePoints(MAP_STATE->actingUnit);
  gMovePath->lastTile = 0;
  MovePath_Truncate(0);
  MovePath_AddTile(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y);
  gMovePath->movesLeft[0] = gMovePath->startMoves;
  MovePath_SetCursor(0xffff, 0xffff);
  MovePath_FollowCursor();
}
