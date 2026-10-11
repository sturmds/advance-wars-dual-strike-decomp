#include "types.h"

extern int data_022a7e68;
void func_01ff8c84(int arg);
int func_0210eb0c(void);
void func_0210eb20(int saved);
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
    func_01ff8c84(0);
    saved = func_0210eb0c();
    n = 0;
    for (i = 1; i < 8; i++) {
        entry = func_0211a54c(i);
        if (entry != NULL) {
            LargeFont_DrawName((u8 *)entry + 2, 6, 5 + n * 2, 0x200 + n * 0x28);
            n++;
        }
    }
    func_0210eb20(saved);
    Gfx_RequestLayerCopy(0);
}
