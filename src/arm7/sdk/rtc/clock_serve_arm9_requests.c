#include "sdk/rtc.h"
#include "sdk/os_arm7.h"

void func_arm7_027d5dbc(u8 *value);
void func_arm7_027d5de8(u8 *value);
void func_arm7_027d5e14(u8 *value);
void func_arm7_027d5e40(u8 *value);
/* These three take u8 * in their definitions; these are the registers they move. */
void func_arm7_027d5e6c(RtcIntRegister *value);
void func_arm7_027d5ec4(RtcStatus1 *value);
void func_arm7_027d5ef0(RtcStatus1 *out);
int func_arm7_027d5f1c(u8 *data);
int func_arm7_027d5f68(u8 *data);
int func_arm7_027d5fb4(u8 *data);
int func_arm7_027d6004(u8 *data);
int func_arm7_027d6054(u8 *data);
int func_arm7_027d60a8(u8 *data);
void func_arm7_027d60fc(u8 *data);
void func_arm7_027d6128(u8 *data);
void func_arm7_027d6154(RtcDateTime *value);
void func_arm7_027d6180(RtcDateTime *out);
void func_arm7_027d61ac(RtcDateTime *value);
void func_arm7_027d61d8(RtcDateTime *out);
void func_arm7_027d6204(u32 hour24);
void func_arm7_027d6304(void);

/*
 * The RTC thread: waits for a request queued by the PXI callback, runs it
 * against the clock with the data in the request buffer, clears the busy
 * flag and sends the result to the ARM9.
 */
void Clock_ServeArm9Requests(void *arg)
{
    OSMessage msg;
    RtcRequestData *data = CLOCK_SHARED_REQUEST_BUF;
    u16 command;

    while (TRUE) {
        MsgQueue_PopOldest_Arm7(&data_arm7_027d81b0.queue, &msg, OS_MESSAGE_BLOCK);
        command = data_arm7_027d81b0.command;
        switch (command) {
        case CLOCK_ASK_RESTART:
            func_arm7_027d6304();
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_RESTART, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_PICK_24H_MODE:
            func_arm7_027d6204(data->regs.status1.hour24);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_PICK_24H_MODE, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_GET_FULL_STAMP:
            func_arm7_027d61d8(&data->dateTime);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_GET_FULL_STAMP, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_GET_CALENDAR:
            func_arm7_027d6180(&data->dateTime);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_GET_CALENDAR, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_GET_HOURS_MINS:
            func_arm7_027d6128(data->regs.value);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_GET_HOURS_MINS, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_GET_INT1_RATE:
            if (!func_arm7_027d60a8(data->regs.value)) {
                data_arm7_027d81b0.busy = FALSE;
                func_arm7_027d58a8(CLOCK_ASK_GET_INT1_RATE, CLOCK_FAIL_INT_MODE_MISMATCH);
            } else {
                data_arm7_027d81b0.busy = FALSE;
                func_arm7_027d58a8(CLOCK_ASK_GET_INT1_RATE, RTC_PXI_RESULT_SUCCESS);
            }
            break;
        case CLOCK_ASK_GET_ALARM_A:
            if (!func_arm7_027d6004(data->regs.value)) {
                data_arm7_027d81b0.busy = FALSE;
                func_arm7_027d58a8(CLOCK_ASK_GET_ALARM_A, CLOCK_FAIL_INT_MODE_MISMATCH);
            } else {
                data_arm7_027d81b0.busy = FALSE;
                func_arm7_027d58a8(CLOCK_ASK_GET_ALARM_A, RTC_PXI_RESULT_SUCCESS);
            }
            break;
        case CLOCK_ASK_GET_ALARM_B:
            if (!func_arm7_027d5f68(data->regs.value)) {
                data_arm7_027d81b0.busy = FALSE;
                func_arm7_027d58a8(CLOCK_ASK_GET_ALARM_B, CLOCK_FAIL_INT_MODE_MISMATCH);
            } else {
                data_arm7_027d81b0.busy = FALSE;
                func_arm7_027d58a8(CLOCK_ASK_GET_ALARM_B, RTC_PXI_RESULT_SUCCESS);
            }
            break;
        case CLOCK_ASK_GET_FLAGS_A:
            func_arm7_027d5ef0(&data->regs.status1);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_GET_FLAGS_A, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_GET_FLAGS_B:
            func_arm7_027d5e98(&data->regs.status2);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_GET_FLAGS_B, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_GET_ADDR3:
            func_arm7_027d5e40(data->regs.value);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_GET_ADDR3, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_GET_ADDR7:
            func_arm7_027d5de8(data->regs.value);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_GET_ADDR7, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_PUT_FULL_STAMP:
            func_arm7_027d61ac(&data->dateTime);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_PUT_FULL_STAMP, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_PUT_CALENDAR:
            func_arm7_027d6154(&data->dateTime);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_PUT_CALENDAR, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_PUT_HOURS_MINS:
            func_arm7_027d60fc(data->regs.value);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_PUT_HOURS_MINS, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_PUT_INT1_RATE:
            if (!func_arm7_027d6054(data->regs.value)) {
                data_arm7_027d81b0.busy = FALSE;
                func_arm7_027d58a8(CLOCK_ASK_PUT_INT1_RATE, CLOCK_FAIL_INT_MODE_MISMATCH);
            } else {
                data_arm7_027d81b0.busy = FALSE;
                func_arm7_027d58a8(CLOCK_ASK_PUT_INT1_RATE, RTC_PXI_RESULT_SUCCESS);
            }
            break;
        case CLOCK_ASK_PUT_ALARM_A:
            if (!func_arm7_027d5fb4(data->regs.value)) {
                data_arm7_027d81b0.busy = FALSE;
                func_arm7_027d58a8(CLOCK_ASK_PUT_ALARM_A, CLOCK_FAIL_INT_MODE_MISMATCH);
            } else {
                data_arm7_027d81b0.busy = FALSE;
                func_arm7_027d58a8(CLOCK_ASK_PUT_ALARM_A, RTC_PXI_RESULT_SUCCESS);
            }
            break;
        case CLOCK_ASK_PUT_ALARM_B:
            if (!func_arm7_027d5f1c(data->regs.value)) {
                data_arm7_027d81b0.busy = FALSE;
                func_arm7_027d58a8(CLOCK_ASK_PUT_ALARM_B, CLOCK_FAIL_INT_MODE_MISMATCH);
            } else {
                data_arm7_027d81b0.busy = FALSE;
                func_arm7_027d58a8(CLOCK_ASK_PUT_ALARM_B, RTC_PXI_RESULT_SUCCESS);
            }
            break;
        case CLOCK_ASK_PUT_FLAGS_A:
            func_arm7_027d5ec4(&data->regs.status1);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_PUT_FLAGS_A, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_PUT_FLAGS_B:
            func_arm7_027d5e6c(&data->regs.status2);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_PUT_FLAGS_B, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_PUT_ADDR3:
            func_arm7_027d5e14(data->regs.value);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_PUT_ADDR3, RTC_PXI_RESULT_SUCCESS);
            break;
        case CLOCK_ASK_PUT_ADDR7:
            func_arm7_027d5dbc(data->regs.value);
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(CLOCK_ASK_PUT_ADDR7, RTC_PXI_RESULT_SUCCESS);
            break;
        default:
            data_arm7_027d81b0.busy = FALSE;
            func_arm7_027d58a8(command, RTC_PXI_RESULT_INVALID_COMMAND);
            break;
        }
    }
}
