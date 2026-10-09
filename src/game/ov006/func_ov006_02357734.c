#include "types.h"
#include "game/Unk_ov006_0235f690.h"

extern Unk_ov006_0235f690 *data_ov006_0236d640;

void func_ov006_02357734(void)
{
    Unk_ov006_0235f690 *state = data_ov006_0236d640;

    if (state->request != 2) {
        return;
    }
    if (state->unk_0c == 0x33) {
        state->unk_0c = 0x3c;
    }
}
