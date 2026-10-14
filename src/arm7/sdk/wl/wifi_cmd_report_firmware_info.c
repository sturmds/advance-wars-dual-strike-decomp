#include "sdk/mi.h"
#include "sdk/wl.h"

u16 WifiBaseband_ChecksumStartValues(void);

extern char gWlVersionString[];

int WifiCmd_ReportFirmwareInfo(WlRequest* request, WlVersionReply* reply) {
  reply->status = 9;
  Mem_CopyHalfwords_Arm7(gWlVersionString, reply->version,
                         sizeof(reply->version));
  reply->chipId = WIFI_MAC_REGISTER(0x000);
  reply->bbChipId = WifiBaseband_Get(0);
  reply->bbCrc = WifiBaseband_ChecksumStartValues();
  reply->rfType = WIFI_DRIVER_STATE->rf.type;
  return 0;
}
