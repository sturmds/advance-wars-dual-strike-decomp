#include "types.h"

void FrameCallback_Remove(void (*callback)(void));
void func_020d3ff4(void);
void func_020d41a4(void);

void func_020d4124(void)
{
    FrameCallback_Remove((void *)func_020d3ff4);
    func_020d41a4();
}
