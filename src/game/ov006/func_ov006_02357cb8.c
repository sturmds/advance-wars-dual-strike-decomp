#include "types.h"
#include "game/Unk_ov006_0235f690.h"

int func_ov006_02364ddc(int a, int b);
int func_ov006_02365754(void);
void func_020d76f0(int id, u8 *buf, int arg);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

void func_ov006_02357cb8(void)
{
    int saved;
    Unk_ov006_0235f690 *state;

    saved = func_ov006_02364ddc(0, 0);
    state = data_ov006_0236d640;
    func_020d76f0(state->unk_38, state->unk_d8, func_ov006_02365754());
    func_ov006_02364ddc(1, saved);
    if (data_ov006_0236d640->unk_00 & 0x100) {
        data_ov006_0236d640->unk_00 &= ~0x100;
        data_ov006_0236d640->unk_d8[0] = 0;
    }
    data_ov006_0236d640->unk_00 &= ~0x1000;
}
