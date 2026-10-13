#ifndef GAME_WIRELESS_H
#define GAME_WIRELESS_H

#include "types.h"

/* Buffers Wireless_Init allocates. Struct tags only, so files with their
 * own WM prototypes need not include sdk/wm.h. */
extern struct WMScanParam* data_0229c8b4;
extern struct WMDataSharingInfo* data_0229c8ac;
extern struct WMDataSet* data_0229c8b0; /* the last received data set */
extern s32 data_0229c89c;               /* MP send buffer size, at least 0x60 */
extern s32 data_0229c8a4;               /* MP receive buffer size */
extern struct WMBssDesc* gWirelessScanBuffer;
extern struct WMBssDesc* gWirelessParents[4]; /* parents found by the scan */

#endif
