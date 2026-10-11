#include "types.h"
#include "game/Unk_02080440.h"

void func_01ff8c84(int layer);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
void Font_LoadDigits(int baseTile);

extern Unk_02080440 *data_027c027c;

void func_02075520(void)
{
    func_01ff8c84(0);
    func_01ff8c84(1);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    func_020c4018();
    Font_LoadDigits(0);
    *data_027c027c->unk_40 = 1;
}
