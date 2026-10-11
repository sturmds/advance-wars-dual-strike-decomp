#include "types.h"
#include "sdk/mb.h"

/* Packs a parent message into dst. Returns the end of the packed bytes, or
 * NULL for a type the parent does not send. Only data messages carry the file
 * id and block number, low byte first. */
u8 *MbComm_WriteParentMessage(const DlPlayHostMsgHead *header, u8 *dst)
{
    u8 *out = dst;

    *out++ = header->type;
    switch (header->type) {
    case DLPLAY_TO_CHILD_UPLOAD_BEGINS:
    case DLPLAY_TO_CHILD_REFUSE:
    case DLPLAY_TO_CHILD_LAYOUT:
    case DLPLAY_TO_CHILD_START_PROGRAM:
    case DLPLAY_TO_CHILD_NO_ROOM:
        break;
    case DLPLAY_TO_CHILD_BLOCK:
        *out++ = header->fileId;
        *out++ = (header->fileId & 0xff00) >> 8;
        *out++ = header->block;
        *out++ = (header->block & 0xff00) >> 8;
        break;
    default:
        return NULL;
    }
    return out;
}
