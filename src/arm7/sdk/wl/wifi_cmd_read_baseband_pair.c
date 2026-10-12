#include "sdk/wl.h"

// Reads a wireless baseband register (writes the read command to 0x04808158,
// returns 0x0480815c).
u16 func_arm7_027c8bcc(int reg);

int WifiCmd_ReadBasebandPair(WlRequest *request, WlReply *reply)
{
    reply->status = 3;
    reply->value = func_arm7_027c8bcc(0x13);
    reply->words[0] = func_arm7_027c8bcc(0x35);
    return 0;
}
