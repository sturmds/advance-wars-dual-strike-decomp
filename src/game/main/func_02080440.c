#include "types.h"
#include "game/Unk_02080440.h"

void func_020800c0(void);
int func_0208c21c(void);
void func_020b9540(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u16 data_0229d8c4;

void func_02080440(void)
{
    func_020800c0();
    if ((data_027c027c->unk_48[data_027c0284]->unk_1c4->unk_2a & 0xf) != 0) {
        return;
    }
    func_020b9540();
    if (func_0208c21c() == 2) {
        data_0229d8c4 = 0;
    }
}
