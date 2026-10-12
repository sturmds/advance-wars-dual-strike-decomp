#include "sdk/wl.h"

void func_arm7_027d0708(void);
void func_arm7_027d06b0(void);
void func_arm7_027d0600(void);

// Request unk_10 selects what to reset, one bit per part of the transmitter.
int WifiCmd_ResetTransmitParts(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (request->unk_10 & 1) {
        func_arm7_027d0708();
    }
    if (request->unk_10 & 2) {
        func_arm7_027d06b0();
    }
    if (request->unk_10 & 4) {
        func_arm7_027d0600();
    }
    return 0;
}
