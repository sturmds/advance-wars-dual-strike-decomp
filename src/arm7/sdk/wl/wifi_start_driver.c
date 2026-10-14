#include "sdk/wl.h"
#include "sdk/wm.h"
#include "sdk/mi.h"
#include "sdk/runtime.h"
#include "sdk/pm.h"

#define REG_WIFIWAITCNT (*(vu16*)0x04000206)

void Wifi_ClearSessionArea(u32 value, int limit);
void WifiTask_ClearQueues(void);
void WifiBuffer_SetUpLists(const WlBufferAllocator* allocator);
BOOL WifiFlash_FetchRadioBlock(void);
void WifiFlash_ReadRadioBytes(u32 offset, u32 size, void* dest);
u32 WifiFlash_ValidateRadioBlock(u32* crcs);
void func_arm7_027cb920(void);
void func_arm7_027cb9ac(void);
void WifiStation_ClearTable(void);
void Wifi_CreateAlarms(void);
void WifiRf_ReadChipFormat(void);
void WifiSelfTest_MacRegisters(void);
void func_arm7_027c93a4(void);
void WifiMac_LoadRegisterTable(void);
void WifiRf_LoadStartValues(void);
void WifiSelfTest_Ram(void);
void WifiSelfTest_Baseband(void);
void WifiBaseband_LoadStartValues(void);
void WifiParam_LoadDefaults(void);
void func_arm7_027c9438(void);
void WifiIrq_Install(void);

/*
 * Sets up the driver's work area at params->unk_00 and starts the driver
 * thread. Returns the status word: 0x80 when the wifi block from the
 * firmware fails its check, and then the second part of the setup is skipped.
 */
u16 Wifi_StartDriver(Unk_arm7_038057d4* params) {
  WlWork* work = params->unk_00;
  u32 crcs;
  volatile u32 zero;

  WIFI_DRIVER_STATE = work;
  zero = 0;
  Mem_FillWordsInBlocks_Arm7(zero, work, sizeof(WlWork));
  WIFI_DRIVER_STATE->limits.spiLockId = LockId_Allocate_Arm7();
  WIFI_DRIVER_STATE->limits.unk_e0 = params->unk_18;
  WIFI_DRIVER_STATE->limits.unk_e4 = params->unk_1c / 2;
  if (WIFI_DRIVER_STATE->limits.unk_e4 == 0) {
    WIFI_DRIVER_STATE->limits.unk_e4 = 0xffffffff;
  }
  WifiBuffer_SetUpLists((WlBufferAllocator*)&params->unk_20);
  WifiFlash_FetchRadioBlock();
  *REG_POWCNT2 |= POWCNT2_WIRELESS;
  REG_WIFIWAITCNT = 0x30;
  WIFI_DRIVER_STATE->unk_634 = 3;
  WifiFlash_ReadRadioBytes(0x3c, 2, &WIFI_DRIVER_STATE->unk_63a);
  WifiFlash_ReadRadioBytes(0x3e, 2, &WIFI_DRIVER_STATE->unk_638);
  WIFI_DRIVER_STATE->limits.fromHost = (struct OSMessageQueue*)params->unk_10;
  WIFI_DRIVER_STATE->limits.toHost = (struct OSMessageQueue*)params->unk_14;
  Wifi_ClearSessionArea((u32)params->unk_2c,
                        params->unk_30 / sizeof(WlStation));
  WifiTask_ClearQueues();
  func_arm7_027cb920();
  func_arm7_027cb9ac();
  WifiStation_ClearTable();
  Wifi_CreateAlarms();
  if (WifiFlash_ValidateRadioBlock(&crcs) != WIFI_RADIO_BLOCK_VALID) {
    WIFI_DRIVER_STATE->state.unk_12 |= 0x80;
  } else {
    WifiRf_ReadChipFormat();
    WifiSelfTest_MacRegisters();
    func_arm7_027c93a4();
    WifiMac_LoadRegisterTable();
    WifiRf_LoadStartValues();
    WifiSelfTest_Ram();
    WifiSelfTest_Baseband();
    WifiBaseband_LoadStartValues();
    WifiParam_LoadDefaults();
    func_arm7_027c9438();
  }
  Thread_Spawn_Arm7(&WIFI_DRIVER_STATE->thread,
                    (void (*)(void*))WifiTask_ThreadMain, NULL, params->unk_04,
                    params->unk_08, params->unk_0c);
  Thread_MakeReady_Arm7(&WIFI_DRIVER_STATE->thread);
  WifiIrq_Install();
  return WIFI_DRIVER_STATE->state.unk_12;
}
