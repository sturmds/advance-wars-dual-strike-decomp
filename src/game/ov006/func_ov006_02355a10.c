#include "types.h"
#include "game/Unk_ov006_0235f690.h"

int func_ov006_02357674(void);
void Sound_PlaySe(u32 soundId);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

void func_ov006_02355a10(int choice)
{
    Unk_ov006_0235f690 *state = data_ov006_0236d640;

    if (state->screenState != 51) {
        return;
    }
    if (state->unitMode == 1) {
        if (state->unk_28 != choice) {
            state->unk_28 = choice;
            func_ov006_02357674();
            Sound_PlaySe(0x10001);
        } else {
            Sound_PlaySe(0x10008);
        }
    }
    state = data_ov006_0236d640;
    if (state->unitMode != 0) {
        return;
    }
    if (state->unk_27 != choice) {
        state->unk_26 = state->unk_27;
        data_ov006_0236d640->unk_27 = choice;
        func_ov006_02357674();
        Sound_PlaySe(0x10001);
    } else {
        Sound_PlaySe(0x10008);
    }
}
