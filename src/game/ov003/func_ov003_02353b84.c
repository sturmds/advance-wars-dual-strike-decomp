#include "types.h"

u32 Ov003_MarkRowTiles(u32 co);
int Ov003_DrawAllTiles(int exp);

int func_ov003_02353b84(u32 co) {
  return Ov003_DrawAllTiles(Ov003_MarkRowTiles(co));
}
