#include "types.h"
#include "sdk/mb.h"

extern DlPlayHostLinkArea *data_022aced4;

void MIi_CpuClear16(u16 value, void *dst, u32 size);
int MbParent_SendHeaderOnly(u8 type, u16 bitmap);
int func_02119750(void);
int func_021193fc(void);

/* Children in each comm state, one bit per aid. */
typedef struct DlPlayChildrenByState {
    u16 connected;
    u16 accepted;
    u16 kicked;
    u16 bootRequest;
    u16 memberFull;
} DlPlayChildrenByState;

/* Sends the parent's message for this frame. Kick, start, boot and
 * member-full messages go first; then the download info for accepted
 * children, else the next file block. When there is nothing to send it
 * sends an empty message to every child. */
int MbParent_SendCommData(void)
{
    volatile u16 zero16 = 0;
    DlPlayChildrenByState bitmaps;
    u16 aid;
    int result;

    MIi_CpuClear16(zero16, &bitmaps, sizeof(bitmaps));
    for (aid = 1; aid <= DLPLAY_CHILD_SLOTS; aid++) {
        switch (data_022aced4->commState[aid - 1]) {
        case DLPLAY_CHILD_JOINED:
            bitmaps.connected |= 1 << aid;
            break;
        case DLPLAY_CHILD_ADMITTED:
            bitmaps.accepted |= 1 << aid;
            break;
        case DLPLAY_CHILD_REFUSED:
            bitmaps.kicked |= 1 << aid;
            break;
        case DLPLAY_CHILD_TOLD_TO_BOOT:
            bitmaps.bootRequest |= 1 << aid;
            break;
        case DLPLAY_CHILD_NO_ROOM:
            bitmaps.memberFull |= 1 << aid;
            break;
        }
    }

    if (bitmaps.bootRequest != 0) {
        result = MbParent_SendHeaderOnly(DLPLAY_TO_CHILD_START_PROGRAM, bitmaps.bootRequest);
    } else if (bitmaps.connected != 0) {
        result = MbParent_SendHeaderOnly(DLPLAY_TO_CHILD_UPLOAD_BEGINS, bitmaps.connected);
    } else if (bitmaps.memberFull != 0) {
        result = MbParent_SendHeaderOnly(DLPLAY_TO_CHILD_NO_ROOM, bitmaps.memberFull);
    } else if (bitmaps.kicked != 0) {
        result = MbParent_SendHeaderOnly(DLPLAY_TO_CHILD_REFUSE, bitmaps.kicked);
    } else if (bitmaps.accepted != 0) {
        result = func_02119750();
    } else {
        result = func_021193fc();
    }
    if (result == DLPLAY_NOTHING_TRANSMITTED) {
        return MbParent_SendHeaderOnly(DLPLAY_TO_CHILD_EMPTY, 0xffff);
    }
    return result;
}
