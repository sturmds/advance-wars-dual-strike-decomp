#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void CallbackSlots_Clear(void);
void NameEntry_StartTask(void *arg, int value);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

void func_ov006_023545ac(void)
{
    CallbackSlots_Clear();
    NameEntry_StartTask(data_ov006_0236d640->unk_64, 8);
}
