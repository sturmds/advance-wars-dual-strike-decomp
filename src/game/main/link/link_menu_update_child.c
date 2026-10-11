#include "types.h"
#include "game/task.h"

#define LINK_UNFLAGGED_ACK 0x8f

extern const TaskScriptCmd data_0215fed4[];

void LinkMenu_DrawSecondView(void);
u8 *Link_ReadUnflaggedPacket(int player);
void Link_SendUnflaggedPacket(void *data);
void Wireless_SetChildAids(int childAids);
void func_0207bb48(int start);
void func_020b9890(void);
void MenuButtons_CloseAll(void);
int Task_JumpToLabel(Task *task, int label);
void func_0210f52c(void *dest, int value, int size);

/* Child side of the player list task. When the parent sends an ack (0x8f) holding the connected
 * child aid bits, takes them over, answers with an ack, closes the buttons and starts the
 * data_0215fed4 task after the script's label 255. */
void LinkMenu_UpdateChild(Task *task)
{
    int gotAck = 0;
    u8 *packet;
    u8 ack[23];

    LinkMenu_DrawSecondView();
    while ((packet = Link_ReadUnflaggedPacket(0)) != NULL) {
        if (packet[0] == LINK_UNFLAGGED_ACK) {
            gotAck = 1;
            Wireless_SetChildAids(packet[1]);
            func_0207bb48(0);
        }
    }
    if (gotAck != 1) {
        return;
    }
    func_0210f52c(ack, 0, 23);
    ack[0] = LINK_UNFLAGGED_ACK;
    Link_SendUnflaggedPacket(ack);
    func_020b9890();
    MenuButtons_CloseAll();
    Task_JumpToLabel(task, 255);
    Task_Create(data_0215fed4, task);
}
