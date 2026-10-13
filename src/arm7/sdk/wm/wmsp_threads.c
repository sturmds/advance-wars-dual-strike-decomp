#include "types.h"
#include "sdk/os_arm7.h"
#include "text_data.h"

#pragma section TEXT_DATA begin

/* The threads Wmsp_Init starts: one runs the ARM9's requests
 * (Wmsp_RequestThread), the other the WL indications
 * (Wmsp_WlReceiveThread). */
OSThread gWmspRequestThread = {0};
OSThread gWmspWlReceiveThread = {0};

#pragma section TEXT_DATA end
