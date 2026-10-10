#include "types.h"

u32 func_0210cef0(void);
void func_0210cf48(u16 state);
void func_021153f0(u16 a);
void func_021153e0(u16 a);
int func_02115ad4(int a0, int a1, int a2, int a3, int a4, int a5);

/* Runs func_02115ad4 between a func_021153f0 / func_021153e0 pair that
 * takes the value func_0210cef0 returned (likely an interrupt state: the
 * last call hands it back, like OS_RestoreInterrupts). */
int func_0200c150(int a0, int a1, int a2)
{
    u32 state = func_0210cef0();
    int result;

    func_021153f0(state);
    result = func_02115ad4(a0, a1, a2, 0, 0, 0);
    func_021153e0(state);
    func_0210cf48(state);
    return result;
}
