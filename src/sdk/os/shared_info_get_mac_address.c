#include "types.h"
#include "sdk/user_settings.h"

void Mem_CopyBytes(const void* src, void* dest, u32 size);

/* Copies the console's MAC address, which the ARM7 startup read from the
 * firmware flash into shared main RAM, to mac. */
void SharedInfo_GetMacAddress(u8* mac) {
  Mem_CopyBytes(SHARED_WIRELESS_INFO->macAddress, mac,
                sizeof(SHARED_WIRELESS_INFO->macAddress));
}
