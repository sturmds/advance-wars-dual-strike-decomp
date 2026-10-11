#include "types.h"
#include "sdk/mb.h"

extern DlPlayHostLinkArea *data_022aced4;

void func_0211e120(DlPlayHostMsgHead *header, void *dst);
int func_0211b458(int size, int bitmap, void *data);

/* Sends a parent message that is only a header (no file data) to the
 * children in bitmap. */
int MbParent_SendHeaderOnly(u8 type, u16 bitmap)
{
    DlPlayHostMsgHead header;

    header.type = type;
    func_0211e120(&header, data_022aced4->sendBuffer);
    return func_0211b458(DLPLAY_HOST_MSG_HEAD_BYTES, bitmap, data_022aced4->sendBuffer);
}
