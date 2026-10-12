#include "types.h"
#include "sdk/os.h"

extern OSThreadInfo data_arm7_0380790c;
int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

ThreadSwapHook func_arm7_037fbba8(ThreadSwapHook callback)
{
    int irqState = func_arm7_037fdb80();
    ThreadSwapHook previous = data_arm7_0380790c.switchCallback;

    data_arm7_0380790c.switchCallback = callback;
    func_arm7_037fdb94(irqState);
    return previous;
}
