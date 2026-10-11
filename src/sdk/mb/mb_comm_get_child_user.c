#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

extern DlPlayHostLinkArea *data_022aced4;

void Mem_CopyBytes(const void *src, void *dest, u32 size);

/* Copies a child's user info (color, name) into the work area and returns the
 * copy, or NULL when there is no such child. */
DlPlayPlayerBadge *MbComm_GetChildUser(u16 aid)
{
    OSIntrMode oldMode = OS_DisableInterrupts();

    if (data_022aced4 != NULL && MbComm_IsChildAid(aid)) {
        Mem_CopyBytes(&data_022aced4->childUsers[aid - 1], &data_022aced4->userInfoCopy, sizeof(DlPlayPlayerBadge));
        OS_RestoreInterrupts(oldMode);
        return &data_022aced4->userInfoCopy;
    }
    OS_RestoreInterrupts(oldMode);
    return NULL;
}
