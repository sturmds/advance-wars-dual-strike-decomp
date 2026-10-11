#include "types.h"
#include "game/Unk_ov006_0235f690.h"

const void *Text_Get(u32 textId);
char *strcpy(char *dest, const char *src);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

/* Copies the text 0x7e000015 + id (id 0 to 2) into the unk_d8 buffer. */
void Ov006_LoadIdText(int id)
{
    switch (id) {
    case 0:
        strcpy((char *)data_ov006_0236d640->unk_d8, Text_Get(0x7e000015));
        break;
    case 1:
        strcpy((char *)data_ov006_0236d640->unk_d8, Text_Get(0x7e000016));
        break;
    case 2:
        strcpy((char *)data_ov006_0236d640->unk_d8, Text_Get(0x7e000017));
        break;
    }
}
