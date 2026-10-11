#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/touch.h"

void func_020fbb58(int x, int y, s32 *out);

extern Unk_02080440 *data_027c027c;
extern u32 data_027c0284;
extern Unk_027c0288 data_027c0288;
extern Unk_ov006_0235f690 *data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* The screen is being touched and the touch began this frame. */
static inline BOOL IsTouchStarted(void)
{
    if (gTouch.active != 0 && gTouch.phase == TOUCH_PRESSED) {
        return TRUE;
    }
    return FALSE;
}

/* The Design Room's copy of MapCursor_UpdateFromTouch: turns the touch in
 * touchX, touchY into a map tile (unk_28, unk_2a), ignoring the top 33 pixel
 * rows. On the 3D screen the tile only changes while request is 1, and a new
 * touch only remembers its tile (unk_30, unk_32) then. No calls and no sound,
 * unlike the main version. The (s16) casts on touchX and touchY are needed
 * to match: the original reads them signed. */
void Ov006_UpdateCursorFromTouch(void)
{
    s32 pos[3];
    int touchX;
    int touchY;

    if (!(data_ov006_0236d640->touch & 0x8000)) {
        return;
    }
    touchY = (s16)data_ov006_0236d640->touchY;
    touchX = (s16)data_ov006_0236d640->touchX;
    if (touchY <= 32) {
        return;
    }
    if (data_027c027c != data_027c0288.unk_00) {
        STATE->unk_28 = (touchX + STATE->unk_1c4->scrollX) >> 4;
        STATE->unk_2a = (touchY + STATE->unk_1c4->scrollY) >> 4;
    } else {
        func_020fbb58(touchX, touchY, pos);
        pos[1] = -pos[1];
        pos[2] = 8;
        if (data_ov006_0236d640->request == 1) {
            STATE->unk_28 = pos[0] >> 9;
            STATE->unk_2a = pos[1] >> 9;
        }
    }
    if (IsTouchStarted() && data_ov006_0236d640->request == 1) {
        STATE->unk_30 = STATE->unk_28;
        STATE->unk_32 = STATE->unk_2a;
    }
    if (STATE->unk_30 != STATE->unk_28 || STATE->unk_32 != STATE->unk_2a) {
        STATE->unk_30 = 0xff;
        STATE->unk_32 = 0xff;
    }
}
