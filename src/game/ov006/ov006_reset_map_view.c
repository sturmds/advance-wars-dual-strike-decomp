#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"

void func_020c4eb4(void);
void Ov006_NewMap(void);
void func_020cafe4(void);

extern Unk_02080440 *data_027c027c;
extern u32 data_027c0284;
extern Unk_ov006_0235f690 *data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define OV6   data_ov006_0236d640

/* Resets the map view for the Design Room. With keep 0 it also puts the
 * scroll back to the top left and calls Ov006_NewMap. */
void Ov006_ResetMapView(int keep)
{
    STATE->unk_18 = 0;
    func_020c4eb4();
    if (keep == 0) {
        STATE->unk_1c4->scrollX = 0;
        STATE->unk_1c4->scrollY = 0;
        STATE->unk_1c4->unk_22 = 0;
        STATE->unk_1c4->unk_24 = 0;
        STATE->unk_1c4->unk_26 = 0;
        STATE->unk_1c4->unk_28 = 0;
        Ov006_NewMap();
    }
    STATE->unk_28 = 0;
    STATE->unk_2a = 0;
    STATE->unk_2c = 0;
    STATE->unk_2e = 0;
    OV6->unk_16 = 0;
    OV6->unk_18 = 0;
    func_020cafe4();
}
