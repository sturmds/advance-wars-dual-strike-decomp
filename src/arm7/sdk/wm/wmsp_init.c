#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/pxi.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"
#include "bss_run.h"

#pragma section BSS_RUN1 begin
static OSMessage data_arm7_027d6c74[4];
#pragma section BSS_RUN1 end
#pragma section BSS_RUN2 begin
static OSMessage data_arm7_027d6c98[4];
#pragma section BSS_RUN2 end
/* Queues for WL indications, WL command answers and ARM9 requests. */
static OSMessageQueue data_arm7_027d6c60;
#pragma section BSS_RUN2 begin
/* also declared in src/arm7/sdk/wm/wmsp_receive_request.c */
OSMessageQueue data_arm7_027d6ca8;
#pragma section BSS_RUN2 end
#pragma section BSS_RUN1 begin
/* also declared in src/arm7/sdk/wm/wmsp_send_wl_command.c */
OSMessageQueue data_arm7_027d6c84;
#pragma section BSS_RUN1 end
#pragma section BSS_RUN2 begin
static OSMessage data_arm7_027d6cbc[32];
/* 0xf08 bytes no code names, before data_arm7_027d7c44, cut in 3 to keep the sizes ascending */
u8 data_arm7_027d6d3c[0x478];
WmspState data_arm7_027d7c44;
u8 data_arm7_027d76fc[0x548];
u8 data_arm7_027d71b4[0x548];
#pragma section BSS_RUN2 end

extern OSThread gWmspWlReceiveThread;
extern OSThread gWmspRequestThread;

/* Bottom of the first thread's stack and top of the second's. */
extern u8 data_arm7_027d7d3c[];

void Wmsp_ReceiveRequest(int tag, WmspRequest* request, BOOL error);
void Wmsp_WlReceiveThread(void* arg);
void Wmsp_RequestThread(void* arg);
u16 Wifi_StartDriver(Unk_arm7_038057d4* params);

/* Sets up WMSP: its queues, reply mutex and two threads, the ARM9 request callback, then starts WL. */
void Wmsp_Init(Unk_arm7_038057d4* params, Unk_arm7_038057d4_2* priorities) {
  int i;
  u8* work;

  data_arm7_027d7c44.unk_50c = priorities->unk_00;
  data_arm7_027d7c44.unk_510 = params->unk_24;
  data_arm7_027d7c44.unk_514 = params->unk_28;
  data_arm7_027d7c44.buf = NULL;
  MsgQueue_SetUpRing_Arm7(&WMSP_WORK->wlQueue, WMSP_WORK->wlMessages, 2);
  MsgQueue_SetUpRing_Arm7(&data_arm7_027d6c60, data_arm7_027d6c74, 4);
  MsgQueue_SetUpRing_Arm7(&data_arm7_027d6c84, data_arm7_027d6c98, 4);
  MsgQueue_SetUpRing_Arm7(&data_arm7_027d6ca8, data_arm7_027d6cbc, 32);
  params->unk_10 = (u32)&WMSP_WORK->wlQueue;
  params->unk_14 = (u32)&data_arm7_027d6c60;
  data_arm7_027d7c44.threadPriority[0][0] = priorities->unk_08;
  data_arm7_027d7c44.threadPriority[0][1] = priorities->unk_18;
  data_arm7_027d7c44.threadPriority[0][2] = priorities->unk_10;
  data_arm7_027d7c44.threadPriority[1][0] = priorities->unk_04;
  data_arm7_027d7c44.threadPriority[1][1] = priorities->unk_14;
  data_arm7_027d7c44.threadPriority[1][2] = priorities->unk_0c;
  Mutex_Clear_Arm7(&data_arm7_027d813c);

  /* The reply mutex starts where this 0x400-byte stack ends, so the thread gets its address as stack top. */
  Thread_Spawn_Arm7(&gWmspWlReceiveThread, Wmsp_WlReceiveThread, NULL,
                    &data_arm7_027d813c, 0x400, priorities->unk_04);
  Thread_MakeReady_Arm7(&gWmspWlReceiveThread);
  Thread_Spawn_Arm7(&gWmspRequestThread, Wmsp_RequestThread, NULL,
                    data_arm7_027d7d3c, 0x1000, priorities->unk_0c);
  Thread_MakeReady_Arm7(&gWmspRequestThread);

  work = (u8*)WMSP_WORK;
  for (i = 0; i < RSSI_HISTORY_LENGTH; i++) {
    ((WmspState*)(work + WMSP_STATE_OFFSET))->history[i] = 0;
  }
  data_arm7_027d7c44.historyIndex = 0;

  Fifo_SetUp_Arm7();
  Fifo_RegisterTagHandler_Arm7(PXI_FIFO_TAG_WM,
                               (PXIFifoCallback)Wmsp_ReceiveRequest);
  params->unk_18 = 2;
  params->unk_0c = priorities->unk_14;
  Wifi_StartDriver(params);
}
