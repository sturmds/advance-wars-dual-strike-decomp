#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void func_0201e954(void);
void func_01ff8c84(int a);
void Gfx_RequestLayerCopy(int bg);
void func_020c4018(void);
void func_ov006_0235cd04(int id);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

void Ov006_OpenScreen(int id)
{
    func_0201e954();
    func_ov006_0235cd04(id);
    func_01ff8c84(1);
    Gfx_RequestLayerCopy(1);
    func_020c4018();
    data_ov006_0236d640->unk_00 |= 0x1000;
}
