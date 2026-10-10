#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void func_ov006_02354870(void);
void func_ov006_02354a44(void);
extern Unk_ov006_0235f690 *data_ov006_0236d640;

void Ov006_InitSlotButtons(void)
{
    func_ov006_02354870();
    func_ov006_02354a44();
    data_ov006_0236d640->unk_39 = 32;
}
