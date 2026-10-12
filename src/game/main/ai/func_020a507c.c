#include "types.h"

int MoveSearch_FindTileByMechSteps(int x, int y, int range, int arg3);
int Ai_IsFreeCaptureSiteTile(int a, int b, int c);

/* Callback the path search calls for each tile. */
extern int (*data_0229e174)(int a, int b, int c);

int func_020a507c(int x, int y, int range) {
  data_0229e174 = Ai_IsFreeCaptureSiteTile;
  if (data_0229e174(x, y, 0) == 1) {
    return 1;
  }
  return MoveSearch_FindTileByMechSteps(x, y, 0, range);
}
