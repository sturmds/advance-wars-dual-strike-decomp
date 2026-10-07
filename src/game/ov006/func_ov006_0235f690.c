#include "types.h"
#include "game/Unk_ov006_0235f690.h"

int func_020c9704(int player);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

void func_ov006_0235f690(void) {
    data_ov006_0236d640->unk_2c[0] = func_020c9704(1);
    data_ov006_0236d640->unk_2c[1] = func_020c9704(2);
    data_ov006_0236d640->unk_2c[2] = func_020c9704(3);
    data_ov006_0236d640->unk_2c[3] = func_020c9704(4);
}
