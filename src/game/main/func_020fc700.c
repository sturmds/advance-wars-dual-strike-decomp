#include "types.h"

void func_0211a450(void *arg, int mode);
BOOL func_020fc558(void);

void func_020fc700(void *arg, int index)
{
    switch (index) {
    case 7:
        func_0211a450(arg, 3);
        break;
    case 10:
        func_0211a450(arg, 1);
        break;
    case 12:
        func_020fc558();
        break;
    case 14:
        func_0211a450(arg, 2);
        break;
    }
}
