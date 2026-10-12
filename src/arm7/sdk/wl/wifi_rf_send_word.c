#include "sdk/wl.h"

int WifiRf_SendWord(u32 value)
{
    IO_WIRELESS_RADIO_WORD_LO = value;
    IO_WIRELESS_RADIO_WORD_HI = value >> 16;
    return WifiRf_AwaitSerialIdle();
}
