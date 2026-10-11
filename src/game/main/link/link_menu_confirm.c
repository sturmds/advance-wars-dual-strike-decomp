#include "types.h"
#include "game/task.h"
#include "game/link_menu_state.h"

#define LINK_UNFLAGGED_ACK 0x8f
#define SE_CONFIRM 0x10006
#define SE_CANCEL 0x10009

void Sound_PlaySe(u32 soundId);
int func_0207a27c(void);
int func_0207a170(void);
u16 func_0207a1dc(void);
BOOL Wireless_SelectParent(u32 index);
void func_0207bb48(int start);
void func_0207bbf8(void);
void func_0207bc14(void);
void LinkBanner_Create(int id);
void Link_SendUnflaggedPacket(void *data);
void MenuButtons_CloseAll(void);
void MenuButtons_RemoveSelectable(void);
Task *func_0207ca34(void);
void Task_ResumeScript(Task *task);
void func_0210f52c(void *dest, int value, int size);

/* A on the player list, unless a choice was made. In link state 3, button 4
 * runs func_0207bc14 and the other buttons join the parent of that row; on
 * success it starts the banner of the button. The screen locks even when
 * joining fails. In state 4, once func_0207a170 is set and a child is
 * connected, it sends the children an ack holding the connected child bits,
 * closes the buttons and resumes the player list task. */
void LinkMenu_Confirm(void)
{
    u8 ack[23];
    int id;

    if (data_0215fed0->locked == 1) {
        return;
    }
    switch (func_0207a27c()) {
    /* needed to match: the empty cases make the jump table */
    case 0:
    case 1:
    case 2:
    case 5:
        break;
    case 3:
        /* needed to match: an int copy that is narrowed again for the call */
        if ((id = data_0215fed0->selected) == 4) {
            func_0207bc14();
            Sound_PlaySe(SE_CONFIRM);
            MenuButtons_RemoveSelectable();
            LinkBanner_Create(data_0215fed0->selected);
        } else if (Wireless_SelectParent((u8)id) == TRUE) {
            func_0207bbf8();
            Sound_PlaySe(SE_CONFIRM);
            MenuButtons_RemoveSelectable();
            LinkBanner_Create(data_0215fed0->selected);
        } else {
            Sound_PlaySe(SE_CANCEL);
        }
        data_0215fed0->locked = 1;
        return;
    case 4:
        if (func_0207a170() == 0) {
            return;
        }
        if (func_0207a1dc() != 0) {
            func_0207bb48(0);
            func_0210f52c(ack, 0, 23);
            ack[0] = LINK_UNFLAGGED_ACK;
            ack[1] = func_0207a1dc();
            Link_SendUnflaggedPacket(ack);
            MenuButtons_CloseAll();
            Task_ResumeScript(func_0207ca34());
            Sound_PlaySe(SE_CONFIRM);
            data_0215fed0->locked = 1;
            return;
        }
        Sound_PlaySe(SE_CANCEL);
        break;
    }
}
