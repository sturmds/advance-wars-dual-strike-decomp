#include "types.h"

void Players_Reset(void);
void func_020d2194(void);
void func_020da650(void);
void func_020c1444(void);
void func_020c1c40(void);
void func_020e5100(void);
void func_020e531c(void);

void func_020c4fa0(void)
{
    Players_Reset();
    func_020c1444();
    func_020c1c40();
    func_020d2194();
    func_020da650();
    func_020e531c();
    func_020e5100();
}
