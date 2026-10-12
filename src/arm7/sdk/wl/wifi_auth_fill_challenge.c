#include "types.h"
#include "sdk/wl.h"

u8 WL_ReadByte(const void *addr);
u16 Wifi_StepLcg(void);
void func_arm7_027c83c8(int seed);               /* sets WlWork.random.value */
void func_arm7_027cb2a0(int station, int seed);  /* keeps the station's challenge seed */

/*
 * Shared key authentication: picks a nonzero seed from the hardware random
 * register, keeps it for the station and fills the challenge text of frame
 * from it. WifiAuth_VerifyChallengeReply makes the same text again from the seed.
 */
void WifiAuth_FillChallenge(int station, WlAuthFrame *frame)
{
    u16 seed;
    u32 i;
    u16 *text;
    u32 length;

    seed = REG_W_RANDOM + (REG_W_RANDOM << 8);
    if (seed == 0) {
        seed = 1;
    }
    func_arm7_027c83c8(seed);
    func_arm7_027cb2a0(station, seed);
    text = frame->challenge;
    length = WL_ReadByte(&frame->challengeLength);
    for (i = 0; i < length; i += 2) {
        *text++ = Wifi_StepLcg();
    }
}
