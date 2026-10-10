#include "types.h"
#include "game/task.h"
#include "game/Unk_ov006_0235f690.h"

void func_0201e954(void);
void func_01ff8c84(int arg);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
int Ov006_HasNonSpace(u8 *text);
u8 *func_021016e4(void);
void func_02101124(void);
char *func_021246d4(char *dest, const char *src);  /* strcpy */

extern Unk_ov006_0235f690 *data_ov006_0236d640;
extern const TaskScriptCmd data_ov006_0236acc4[];

void func_ov006_02354444(void)
{
    func_0201e954();
    func_01ff8c84(1);
    Gfx_RequestLayerCopy(1);
    func_020c4018();
    if (Ov006_HasNonSpace(data_ov006_0236d640->unk_d8)) {
        func_021246d4((char *)func_021016e4(), (char *)data_ov006_0236d640->unk_d8);
    } else {
        func_02101124();
    }
    data_ov006_0236d640->unk_64 = Task_Create(data_ov006_0236acc4, TASK_LIST(3));
}
