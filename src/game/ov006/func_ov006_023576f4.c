#include "types.h"
#include "game/Unk_ov006_0235f690.h"

extern Unk_ov006_0235f690 *data_ov006_0236d640;

void func_ov006_023576f4(int value)
{
    Unk_ov006_0235f690 *state = data_ov006_0236d640;

    if (state->request != 2) {
        return;
    }
    if (state->screenState != 51) {
        return;
    }
    if (state->unk_29 != value) {
        state->unk_29 = value;
        data_ov006_0236d640->screenState = 70;
    }
}
