#include "types.h"
#include "sdk/wm.h"
#include "sdk/os.h"

/* Signal strength of the current link; 0 when not connected. A parent with no
 * children reports 0. */
u16 WM_GetLinkLevel(void)
{
    WMArm9Buf *buf = func_02116e10();

    if (func_02116df4() != 0) {
        return 0;
    }
    DC_InvalidateRange(buf->status, 2);
    switch (buf->status->state) {
    case WM_STATE_MP_PARENT:
        DC_InvalidateRange(&buf->status->childBitmap, 2);
        if (buf->status->childBitmap == 0) {
            return 0;
        }
        /* fall through */
    case WM_STATE_MP_CHILD:
    case WM_STATE_DCF_CHILD:
        DC_InvalidateRange(&buf->status->linkLevel, 2);
        return buf->status->linkLevel;
    default:
        return 0;
    }
}
