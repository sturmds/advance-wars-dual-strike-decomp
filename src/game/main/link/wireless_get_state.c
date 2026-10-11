#include "types.h"

extern u32 gWirelessState;

/* State of the wireless driver: 4 when parent, 5 when child. */
int Wireless_GetState(void)
{
    return gWirelessState;
}
