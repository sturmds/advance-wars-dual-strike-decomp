#include "types.h"
#include "sdk/wmsp.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

WmspSlot *Wmsp_TakeFlaggedSlot(void)
{
    WmspSlot *found = NULL;
    int i;
    int irqState = func_arm7_037fdb80();
    WmspBuf *buf = data_arm7_027d7c44.buf;

    if (buf != NULL) {
        for (i = 0; i < WMSP_SLOT_COUNT; i++) {
            if (buf->slots[i].flags & WMSP_SLOT_FLAG) {
                found = &buf->slots[i];
                buf->slots[i].flags &= ~WMSP_SLOT_FLAG;
                break;
            }
        }
    }
    func_arm7_037fdb94(irqState);
    return found;
}
