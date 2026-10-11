#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_FillWhite(void);
void func_02007568(void);
void func_0200a6d0(void *a);
void func_02009ac0(void);
void Screen_ApplyAll(void);
void func_01ff9b48(void);
void func_020f8994(void);
void func_020f88b8(void);
void func_020f88ec(void);
void func_020f8848(void);
void func_020f882c(void);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern u8 data_02166dc8[];

void func_020f869c(int mode)
{
    data_027c0280->masterBrightness = -16;
    if (mode == 1) {
        func_02007568();
    }
    if (mode == 0) {
        Screen_FillWhite();
    }
    func_0200a6d0(data_02166dc8);
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 0;
    func_02009ac0();
    if (data_027c027c->unk_50[data_027c0284]->callback != NULL) {
        data_027c027c->unk_50[data_027c0284]->callback();
    }
    Screen_ApplyAll();
    func_01ff9b48();
    func_020f8994();
    func_020f88b8();
    func_020f88ec();
    func_020f8848();
    func_020f882c();
    data_027c0280->masterBrightness = 0;
}
