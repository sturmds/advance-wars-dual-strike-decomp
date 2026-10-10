#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_LoadCoPreview(void);
void Ov006_LoadEntryTerrainGraphics(void);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

void func_ov006_02352764(void)
{
    data_ov006_0236d640->unk_1c = 1;
    data_ov006_0236d640->unk_25 = 1;
    data_ov006_0236d640->unk_1a = 1;
    data_ov006_0236d640->unk_24 = 1;
    Ov006_LoadCoPreview();
    Ov006_LoadEntryTerrainGraphics();
}
