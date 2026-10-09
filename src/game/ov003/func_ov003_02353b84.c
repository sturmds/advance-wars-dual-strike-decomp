#include "types.h"

u32 func_ov003_02356230(u32 co);
int Ov003_DrawAllTiles(int exp);

int func_ov003_02353b84(u32 co)
{
    return Ov003_DrawAllTiles(func_ov003_02356230(co));
}
