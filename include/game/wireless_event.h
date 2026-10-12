#ifndef GAME_WIRELESS_EVENT_H
#define GAME_WIRELESS_EVENT_H

#include "types.h"

/* Argument of the wireless callbacks: the u16 at +2 is the error code,
 * 0 on success. */
typedef struct WirelessEvent {
  u16 apiId;
  u16 errorCode;
} WirelessEvent;

#endif
