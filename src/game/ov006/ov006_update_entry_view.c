#include "types.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/unlocks.h"

void func_ov006_02353c28(void);
void func_ov006_02353d44(void);
void func_ov006_02357734(void);

extern Unk_ov006_0235f690 *data_ov006_0236d640;
extern s8 data_ov006_0236a7a0[];
extern s8 data_ov006_0236a7b4[];
extern s8 data_ov006_0236a7d0[];

#define STATE data_ov006_0236d640

/* Updates the view of entry `index`, depending on STATE->unk_29. With 1: unk_1a
 * from a table picked by game flag 0x2b, redrawn by func_ov006_02353c28 when it
 * or unk_24 changed. With 0: unk_1c from data_ov006_0236a7a0, redrawn by
 * func_ov006_02353d44 when it changed; for ids 6, 8, 10, 11, 14 and 22 unk_27
 * goes in bits 5 and up. Both set bit 3 of the flags. */
void Ov006_UpdateEntryView(int index)
{
    int a;
    int shift;

    if (STATE->unk_29 == 1) {
        if (GameFlag_IsSet(0x2b) == 1) {
            a = data_ov006_0236a7d0[index];
        } else {
            a = data_ov006_0236a7b4[index];
        }
        if (STATE->unk_1a != a
            || STATE->unk_24 != STATE->unk_28) {
            STATE->unk_1a = a;
            STATE->unk_1e = index;
            STATE->unk_24 = STATE->unk_28;
            func_ov006_02353c28();
        }
        func_ov006_02357734();
        STATE->unk_00 |= 8;
    }
    if (STATE->unk_29 != 0) {
        return;
    }
    shift = STATE->unk_27;
    a = data_ov006_0236a7a0[index];
    if (shift == 0 && a == 8) {
        return;
    }
    switch (a) {
    case 6:
    case 8:
    case 10:
    case 11:
    case 14:
    case 22:
        a = (a & 31) | (shift << 5);
        break;
    }
    if (STATE->unk_1c != a) {
        STATE->unk_1c = a;
        STATE->unk_20 = index;
        STATE->unk_25 = STATE->unk_27;
        func_ov006_02353d44();
    }
    func_ov006_02357734();
    STATE->unk_00 |= 8;
}
