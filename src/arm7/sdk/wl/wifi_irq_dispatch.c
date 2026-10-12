#include "sdk/wl.h"

void WifiIrq_TxBegun(void);
void WifiIrq_RxBegun(void);
void WifiIrq_BeforeBeacon(void);
void WifiIrq_BeaconSent(void);
void WifiIrq_AfterBeacon(void);
int func_arm7_027cab80(void); /* acknowledges W_IRQ_RF_WAKEUP */
void WifiIrq_TxErrorCounted(void);
void WifiIrq_RxEventCounted(void);
void func_arm7_037f9e28(void);
void WifiIrq_StatsHalfFull(void);
void WifiIrq_TxFinished(void);
void func_arm7_037fa0b4(int arg);

/* Wireless interrupt handler: runs the handler of each pending, enabled
   cause until none is left. */
void WifiIrq_Dispatch(void)
{
    int causes;

    for (;;) { /* needed to match: a while loop tests at the bottom */
        causes = WIFI_IRQ_PENDING_FLAGS & WIFI_IRQ_ENABLE_MASK;
        if (causes == 0) {
            break;
        }
        if (causes & W_IRQ_TX_START) {
            WifiIrq_TxBegun();
        }
        if (causes & W_IRQ_RX_START) {
            WifiIrq_RxBegun();
        }
        if (causes & W_IRQ_PRE_BEACON) {
            WifiIrq_BeforeBeacon();
        }
        if (causes & W_IRQ_BEACON) {
            WifiIrq_BeaconSent();
        }
        if (causes & W_IRQ_POST_BEACON) {
            WifiIrq_AfterBeacon();
        }
        if (causes & W_IRQ_RF_WAKEUP) {
            func_arm7_027cab80();
        }
        if (causes & W_IRQ_TX_COUNT) {
            WifiIrq_TxErrorCounted();
        }
        if (causes & W_IRQ_RX_COUNT) {
            WifiIrq_RxEventCounted();
        }
        if (causes & W_IRQ_RX_DONE) {
            func_arm7_037f9e28();
        }
        if (causes & W_IRQ_COUNT_OVERFLOW) {
            WifiIrq_StatsHalfFull();
        }
        if (causes & W_IRQ_TX_DONE) {
            WifiIrq_TxFinished();
        }
        if (causes & W_IRQ_MP_END) {
            func_arm7_037fa0b4(1);
        }
    }
    OS7_IRQ_CHECK_FLAGS |= IRQ_MASK_WIRELESS;
}
