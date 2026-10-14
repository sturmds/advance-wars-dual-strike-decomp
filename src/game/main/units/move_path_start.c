#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/move_path.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern u8 data_021492c0[];
extern MovePath* gMovePath;

void TransferQueue_Add(const void* src, void* dst, u32 size);
void IconBank_LoadPalette(int bank, int paletteSlot);
int Unit_GetMovePoints(MapUnit* unit);
void MovePath_Truncate(int count);
void MovePath_AddTile(s8 x, s8 y);
void MovePath_SetCursor(int a, int b);
void MovePath_FollowCursor(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Starts a move path for the acting unit: loads the path graphics unless
 * data_027c027c and data_027c0288 hold the same pointer, empties the path,
 * puts the unit's own tile first with its full move points, and runs
 * MovePath_FollowCursor. */
void MovePath_Start(void) {
  if (data_027c027c != data_027c0288.unk_00) {
    TransferQueue_Add(data_021492c0, data_027c0280->objVram + 0x3280, 0xa00);
    IconBank_LoadPalette(1, 19);
  }
  gMovePath->startMoves = Unit_GetMovePoints(GAME->actingUnit);
  gMovePath->lastTile = 0;
  MovePath_Truncate(0);
  MovePath_AddTile(GAME->actingUnit->x, GAME->actingUnit->y);
  gMovePath->movesLeft[0] = gMovePath->startMoves;
  MovePath_SetCursor(0xffff, 0xffff);
  MovePath_FollowCursor();
}
