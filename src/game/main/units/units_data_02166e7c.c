#include "types.h"
#include "game/move_path.h"

extern u8 data_022a6c30[];

MovePath* gMovePath = (MovePath*)&data_022a6c30;
u16 gMovePathArrowCells[5][5] = {
#include "data/main/gMovePathArrowCells.inc.c"
};
