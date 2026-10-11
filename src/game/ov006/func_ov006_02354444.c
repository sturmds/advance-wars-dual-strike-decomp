#include "types.h"
#include "game/task.h"
#include "game/Unk_ov006_0235f690.h"

void func_0201e954(void);
void Gfx_ClearLayerMap(int arg);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
int Ov006_HasNonSpace(u8 *text);
u8 *func_021016e4(void);
void NameEntry_ClearBuffer(void);
char *strcpy(char *dest, const char *src);

extern Unk_ov006_0235f690 *data_ov006_0236d640;
extern const TaskScriptCmd data_ov006_0236acc4[];

void func_ov006_02354444(void)
{
    func_0201e954();
    Gfx_ClearLayerMap(1);
    Gfx_RequestLayerCopy(1);
    func_020c4018();
    if (Ov006_HasNonSpace(data_ov006_0236d640->unk_d8)) {
        strcpy((char *)func_021016e4(), (char *)data_ov006_0236d640->unk_d8);
    } else {
        NameEntry_ClearBuffer();
    }
    data_ov006_0236d640->unk_64 = Task_Create(data_ov006_0236acc4, TASK_LIST(3));
}
