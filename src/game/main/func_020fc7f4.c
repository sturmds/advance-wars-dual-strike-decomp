#include "types.h"

extern int data_022a7e68;
void Gfx_ClearLayerMap(int arg);
int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int saved);
void *func_0211a54c(u16 index);
void LargeFont_DrawName(void *dest, int a, u16 b, int c);
void Gfx_RequestLayerCopy(int layer);

void func_020fc7f4(void)
{
    int saved;
    int i;
    int n;
    void *entry;

    if (data_022a7e68 != 2) {
        return;
    }
    Gfx_ClearLayerMap(0);
    saved = OS_DisableInterrupts();
    n = 0;
    for (i = 1; i < 8; i++) {
        entry = func_0211a54c(i);
        if (entry != NULL) {
            LargeFont_DrawName((u8 *)entry + 2, 6, 5 + n * 2, 0x200 + n * 0x28);
            n++;
        }
    }
    OS_RestoreInterrupts(saved);
    Gfx_RequestLayerCopy(0);
}
