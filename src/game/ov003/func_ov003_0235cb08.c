#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov003_0235dd38.h"
#include "game/ov003_unit.h"

extern Unk_027c0280 *data_027c0280;
void func_ov003_0235d850(int rowNumber);

void func_ov003_0235cb08(Unk_ov003_0235dd38 *obj)
{
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
    /* clears the high nibble of unk_1da */
    data_ov003_02364928->unk_1da &= ~0xf0;
    func_ov003_0235d850(obj->unk_52);
}
