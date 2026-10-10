#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_LoadSlotTiles(void);
void func_ov006_02351bf0(void);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

/* Takes a request posted by Ov006_PostRequest (flag 0x8000): clears the flag
 * and unk_0c, then runs Ov006_LoadSlotTiles. While unk_0c is 0, also runs
 * func_ov006_02351bf0. */
void func_ov006_02350560(void)
{
    if (data_ov006_0236d640->unk_00 & 0x8000) {
        data_ov006_0236d640->unk_00 &= ~0x8000;
        data_ov006_0236d640->unk_0c = 0;
        Ov006_LoadSlotTiles();
    }
    if (data_ov006_0236d640->unk_0c == 0) {
        func_ov006_02351bf0();
    }
}
