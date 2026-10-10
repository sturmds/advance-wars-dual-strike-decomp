#include "types.h"

int func_ov006_02357694(void);
void func_ov006_02355a10(int choice);

/* Maps button ids 96 to 100 to a choice number and passes it on. */
void func_ov006_02355ac4(u8 *buttonId)
{
    int choice = 1;

    if (func_ov006_02357694() == 0) {
        return;
    }
    switch (*buttonId) {
    case 96:
        choice = 1;
        break;
    case 97:
        choice = 2;
        break;
    case 98:
        choice = 4;
        break;
    case 99:
        choice = 3;
        break;
    case 100:
        choice = 0;
        break;
    }
    func_ov006_02355a10(choice);
}
