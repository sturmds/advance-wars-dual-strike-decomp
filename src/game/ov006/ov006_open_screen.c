#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void func_0201e954(void);
void Gfx_ClearLayerMap(int a);
void Gfx_RequestLayerCopy(int bg);
void func_020c4018(void);
void Ov006_FillMapForScreen(int id);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

void Ov006_OpenScreen(int id)
{
    func_0201e954();
    Ov006_FillMapForScreen(id);
    Gfx_ClearLayerMap(1);
    Gfx_RequestLayerCopy(1);
    func_020c4018();
    data_ov006_0236d640->unk_00 |= 0x1000;
}
