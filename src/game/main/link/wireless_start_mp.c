#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/wireless_event.h"

extern void* data_0229c884;
extern void* data_0229c890;
extern u32 data_0229c89c;
extern u32 data_0229c8a4;

void Wireless_OnPortReceive(void* event);
void Wireless_OnMpEvent(WirelessEvent* event);
int WM_SetPortCallback(int port, void (*callback)(void* event), int arg);
int WM_StartMPEx(void (*callback)(WirelessEvent* event), void* recvBuf,
                 u16 recvBufSize, void* sendBuf, u16 sendBufSize, u16 frequency,
                 int arg6, int arg7, int arg8, int arg9, int arg10);

/* Sets Wireless_OnPortReceive as the receive callback of port 12, then starts
 * the multi-player transfer with the buffers in data_0229c884 to
 * data_0229c8a4. True when the request started. */
BOOL Wireless_StartMp(void) {
  if (WM_SetPortCallback(12, Wireless_OnPortReceive, 0) != 0) {
    return FALSE;
  }
  return WM_StartMPEx(Wireless_OnMpEvent, data_0229c890, data_0229c8a4,
                      data_0229c884, data_0229c89c, data_0215fe84->unk_0c, 0, 0,
                      0, 0, 0) == 2;
}
