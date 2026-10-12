#include "types.h"
#include "sdk/os.h"
#include "sdk/wmsp.h"

extern OSMessageQueue data_arm7_027d6c84;

BOOL func_arm7_037fc44c(OSMessageQueue *queue, OSMessage message, int flags);
BOOL func_arm7_037fc3ac(OSMessageQueue *queue, OSMessage *message, int flags);
void SndHw_Disable(void);
void func_arm7_037fdc10(void);

WlCommand *Wmsp_SendWlCommand(WlCommand *command)
{
    WlCommand *answer;

    func_arm7_037fc44c(&WMSP_WORK->wlQueue, command, OS_MESSAGE_BLOCK);
    func_arm7_037fc3ac(&data_arm7_027d6c84, (OSMessage *)&answer, OS_MESSAGE_BLOCK);
    if ((answer->words + 2)[answer->index] == 14) {
        RadioNotice *indication = func_arm7_03807640();

        indication->apiid = WM_APIID_INDICATION;
        indication->errcode = WM_ERRCODE_FLASH_ERROR;
        indication->state = 23;
        func_arm7_038076e8(indication);
        SndHw_Disable();
        func_arm7_037fdc10();
    }
    return answer;
}
