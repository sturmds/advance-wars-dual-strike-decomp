#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_LoadCoIcons(int arg);
void func_ov006_02352048(int arg);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

void func_ov006_02352708(int arg)
{
    if (arg == 1) {
        Ov006_LoadCoIcons(data_ov006_0236d640->unk_28);
    } else if (arg == 0) {
        func_ov006_02352048(data_ov006_0236d640->unk_27);
    }
}
