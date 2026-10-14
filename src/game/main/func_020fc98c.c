#include "types.h"
#include "sdk/mb.h"
#include "game/wireless_host.h"

extern int data_022a7e68;
extern void* data_022a7e6c;
/* also declared in src/game/main/wireless_host_fill_info.c */
WirelessHostUser gWirelessHostUser;
WirelessHostGame data_022a7e88;

#define VBLANK_COUNT_ADDR 0x027ffc3c

void Sound_StopSong(void);
void Sound_Shutdown(void);
void* Mem_Alloc(u32 size);
void WirelessHost_FillInfo(void);
void DownloadPlay_SetUpWork(void* work, WirelessHostUser* user, u32 id,
                            u16 tick, int dma);
void DownloadPlay_SetSendSizeAndChildren(int size, int count);
void func_020fc700(u32 aid, u32 state, void* arg);
BOOL func_020fc698(void);

/* Stops the sound, then sets up the wireless host with a DLPLAY_WORK_SIZE-byte work
 * area and state 3. */
void DownloadPlay_Start(void) {
  u32 tick;

  Sound_StopSong();
  Sound_Shutdown();
  data_022a7e6c = Mem_Alloc(DLPLAY_WORK_SIZE);
  data_022a7e68 = 3;
  WirelessHost_FillInfo();
  /* needed to match: volatile read keeps the tick load before the other arguments */
  tick = *(volatile u32*)VBLANK_COUNT_ADDR;
  DownloadPlay_SetUpWork(data_022a7e6c, &gWirelessHostUser, WIRELESS_HOST_ID,
                         tick, 2);
  DownloadPlay_SetSendSizeAndChildren(0x100, 8);
  MbComm_SetCallback(func_020fc700);
  func_020fc698();
}
