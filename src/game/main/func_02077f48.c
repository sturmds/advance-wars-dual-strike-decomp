#include "types.h"
#include "game/Unk_0215fe84.h"
extern u32 data_0229c88c;
/* Probably an SDK call: the second argument is passed on to the callback. */
void func_02118f34(void (*callback)(void), int mode);
void func_02077fc8(void);
void func_02077fe8(void);

void func_02077f48(int mode){
 if (data_0229c88c != 4) return;
 if (mode == data_0215fe84->unk_18) return;
 if (mode == 1) { func_02118f34(func_02077fe8, mode); return; }
 func_02118f34(func_02077fc8, mode);
}
