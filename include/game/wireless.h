#ifndef GAME_WIRELESS_H
#define GAME_WIRELESS_H

#include "types.h"

/* Buffers Wireless_Init allocates. Struct tags only, so files with their
 * own WM prototypes need not include sdk/wm.h. */
extern struct WMScanParam* data_0229c8b4;
extern struct WMDataSharingInfo* data_0229c8ac;
extern struct WMDataSet* data_0229c8b0; /* the last received data set */
extern s32 data_0229c89c;               /* MP send buffer size, at least 0x60 */
#define WIRELESS_MP_SEND_SIZE 0x120     /* Wireless_Init sets data_0229c89c */
#define WIRELESS_MP_RECEIVE_SIZE 0x940  /* Wireless_Init sets data_0229c8a4 */
extern s32 data_0229c8a4;               /* MP receive buffer size */
extern struct WMBssDesc* gWirelessScanBuffer;
/* Bytes of the child SSID sent with a connect request; it holds this
 * console's nickname (Wireless_ResetState). */
#define WIRELESS_SSID_SIZE 24
extern u8 data_0229c8f8[WIRELESS_SSID_SIZE];

/* Parents a scan keeps, and their link levels. */
#define WIRELESS_PARENT_COUNT 4
extern struct WMBssDesc* gWirelessParents[];
extern u8 gWirelessParentLinkLevels[WIRELESS_PARENT_COUNT];

#endif
