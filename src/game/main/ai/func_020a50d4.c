#include "types.h"

int MoveSearch_FindTile(int x, int y, int unitType, int movement);
int func_020a633c(int a, int b, int c);

/* Callback the path search at MoveSearch_FindTile calls for each tile. */
extern int (*data_0229e174)(int a, int b, int c);

/* Runs the path search with the func_020a633c callback installed. */
int func_020a50d4(int x, int y, int unitType, int movement)
{
    data_0229e174 = func_020a633c;
    return MoveSearch_FindTile(x, y, unitType, movement);
}
