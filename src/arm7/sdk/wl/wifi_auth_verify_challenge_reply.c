#include "types.h"
#include "sdk/wl.h"

u8 WL_ReadByte(const void *addr);
u16 Wifi_StepLcg(void);
void func_arm7_027c83c8(int seed);        /* sets WlWork.random.value */
u16 func_arm7_027cb0c8(u32 station);      /* station's challenge seed */

/*
 * Shared key authentication: checks the challenge text the station sent back
 * against the text made from the seed kept for it. TRUE when they match.
 */
BOOL WifiAuth_VerifyChallengeReply(WlAuthFrame *frame)
{
    u32 i;
    u32 length;
    u16 *text;

    func_arm7_027c83c8(func_arm7_027cb0c8(frame->station));
    text = frame->challenge;
    length = WL_ReadByte(&frame->challengeLength);
    for (i = 0; i < length / 2; i++) {
        if (*text++ != Wifi_StepLcg()) {
            return FALSE;
        }
    }
    if (length & 1) {
        if ((*text & 0xff) != (Wifi_StepLcg() & 0xff)) {
            return FALSE;
        }
    }
    return TRUE;
}
