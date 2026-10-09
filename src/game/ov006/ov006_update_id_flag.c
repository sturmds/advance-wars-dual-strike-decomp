#include "types.h"
#include "game/Unk_ov006_0235f690.h"

int func_020d7b04(int id, u8 *buf);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

/* Sets (when func_020d7b04 returns 1 for the id) or clears flag bit 9, 10 or 11
 * for id 0, 1 or 2. Always returns 0. */
int Ov006_UpdateIdFlag(int id)
{
    int result;

    /* Needed to match: result is set to 0 inside each branch. */
    if (func_020d7b04((u8)id, &data_ov006_0236d640->unk_eb) == 1) {
        result = 0;
        switch (id) {
        case 0:
            data_ov006_0236d640->unk_00 |= 0x200;
            break;
        case 1:
            data_ov006_0236d640->unk_00 |= 0x400;
            break;
        case 2:
            data_ov006_0236d640->unk_00 |= 0x800;
            break;
        }
    } else {
        result = 0;
        switch (id) {
        case 0:
            data_ov006_0236d640->unk_00 &= ~0x200;
            break;
        case 1:
            data_ov006_0236d640->unk_00 &= ~0x400;
            break;
        case 2:
            data_ov006_0236d640->unk_00 &= ~0x800;
            break;
        }
    }
    return result;
}
