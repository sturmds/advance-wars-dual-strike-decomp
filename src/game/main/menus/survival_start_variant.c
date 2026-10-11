#include "types.h"
#include "game/Unk_022a7b80.h"

extern Unk_022a7b80 data_022a7b80;

void func_021028e4(void);
void func_021029a8(void);
void func_02102a6c(void);

/* Starts a Survival run by the variant in data_022a7b80.unk_07: 0 and 3,
 * 1 and 4, and 2 and 5 pair up (as in Survival_FinishMap), each pair with its
 * own setup routine. */
void Survival_StartVariant(void)
{
    switch (data_022a7b80.unk_07) {
    case 1:
    case 4:
        func_02102a6c();
        break;
    case 2:
    case 5:
        func_021029a8();
        break;
    case 0:
    case 3:
        func_021028e4();
        break;
    }
}
