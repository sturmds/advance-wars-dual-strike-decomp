#include "types.h"

u32 func_0210cef0(void);
void func_0210cf48(u16 state);
void func_021153f0(u16 a);
void func_021153e0(u16 a);
void func_02115a30(int a);
int func_02115c74(int a0, void *a1, int a2, int a3, int a4, int a5);

int func_0200c2a0(void)
{
    u32 state = func_0210cef0();
    int result;
    u32 buffer;

    func_02115a30(0x1202);
    func_021153f0(state);
    result = func_02115c74(0, &buffer, 4, 0, 0, 0);
    func_021153e0(state);
    func_0210cf48(state);
    return result;
}
