#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void *EventScript_Start(const void *script);
int func_020d7b04(int id, u8 *buf);
void Ov006_ClearLayer1Box(void);
void Ov006_ClearLayer0Box(void);
void func_ov006_023583d8(void);
int Ov006_HasNonSpace(u8 *buf);
void func_ov006_02357bcc(int id);

extern Unk_ov006_0235f690 *data_ov006_0236d640;
extern u8 data_ov006_0236bb60[];
extern u8 data_ov006_0236bc00[];
extern u8 data_ov006_0236bca0[];
extern u8 data_ov006_0236bd50[];

void Ov006_StartIdScript(int id)
{
    Ov006_ClearLayer1Box();
    Ov006_ClearLayer0Box();
    func_ov006_023583d8();
    data_ov006_0236d640->unk_00 &= ~0x100;
    data_ov006_0236d640->unk_38 = id;
    if (func_020d7b04(data_ov006_0236d640->unk_38, &data_ov006_0236d640->unk_eb) == 0) {
        if (Ov006_HasNonSpace(data_ov006_0236d640->unk_d8) == 0) {
            func_ov006_02357bcc(id);
            data_ov006_0236d640->unk_00 |= 0x100;
            EventScript_Start(data_ov006_0236bca0);
        } else {
            EventScript_Start(data_ov006_0236bb60);
        }
    } else {
        if (Ov006_HasNonSpace(data_ov006_0236d640->unk_d8) == 0) {
            func_ov006_02357bcc(id);
            data_ov006_0236d640->unk_00 |= 0x100;
            EventScript_Start(data_ov006_0236bd50);
        } else {
            EventScript_Start(data_ov006_0236bc00);
        }
    }
}
