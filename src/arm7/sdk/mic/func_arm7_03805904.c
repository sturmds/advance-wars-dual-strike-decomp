#include "sdk/mic.h"

/*
 * One step of auto sampling, run from the timer 3 interrupt: reads a sample
 * (unless the SPI bus is busy), stores it into the buffer and the shared
 * last-sample word, and at the end of the buffer either starts over and sends
 * MIC_PXI_NOTICE_BUFFER_FULL (MIC_TYPE_LOOP) or queues an AUTO_STOP request.
 *
 * Hand-written assembly: it pushes five registers with no frame padding,
 * which mwcc 2.0 never emits, and branches on flags set before a store.
 *
 * MICWork offsets: 0x20 status, 0x24 type, 0x26 mode, 0x28 buffer,
 * 0x2c offset, 0x30 size, 0x38 pendingSample, 0x3a lastSample.
 * MICSystemWork offsets: 0x390 lastAddress, 0x394 lastSample.
 */
#ifdef __MWERKS__
asm void func_arm7_03805904(void)
{
    stmfd sp!, {r4, r5, r6, r7, lr}
    ldr r4, =data_arm7_0380af74
    ldrh r5, [r4, #0x26]
    and r0, r5, #MIC_TYPE_NO_REPEAT
    cmp r0, #MIC_TYPE_NO_REPEAT
    ldrh r6, [r4, #0x38]
    ldrneh r7, [r4, #0x3a]
    ldreq r7, =0xffff
    bl SPIi_CheckEntry
    cmp r0, #0
    bne store
    mov r0, #SERIAL_OWNER_MICROPHONE
    bl SPIi_CheckException
    cmp r0, #0
    beq store
    and r0, r5, #MIC_TYPE_12BIT
    cmp r0, #MIC_TYPE_12BIT
    bne read_8bit
    bl func_arm7_038060bc
    tst r5, #MIC_TYPE_SIGNED
    moveq r7, r0
    eorne r7, r0, #0x8000
    b store
read_8bit:
    bl func_arm7_03806184
    tst r5, #MIC_TYPE_SIGNED
    moveq r7, r0
    eorne r7, r0, #0x80
store:
    and r0, r5, #MIC_TYPE_12BIT
    ldr r3, =0x027ffc00
    ldr r1, [r4, #0x2c]
    cmp r0, #MIC_TYPE_12BIT
    bne store_8bit
    ldr r2, [r4, #0x28]
    strh r7, [r2, r1]!
    str r2, [r3, #0x390]
    add r3, r3, #0x394
    strh r7, [r3]
    add r1, r1, #2
    b advance
store_8bit:
    and r7, r7, #0xff
    tst r1, #1
    bne store_pair
    mov r6, r7
    add r1, r1, #1
    b advance
store_pair:
    orr r0, r6, r7, lsl #8
    ldr r2, [r4, #0x28]
    sub r1, r1, #1
    strh r0, [r2, r1]!
    str r2, [r3, #0x390]
    add r3, r3, #0x394
    strh r0, [r3]
    add r1, r1, #2
advance:
    strh r6, [r4, #0x38]
    strh r7, [r4, #0x3a]
    ldr r0, [r4, #0x30]
    cmp r1, r0
    movcs r1, #0
    str r1, [r4, #0x2c]
    bcc done
    ldrh r0, [r4, #0x24]
    and r0, r0, #MIC_TYPE_LOOP
    cmp r0, #MIC_TYPE_LOOP
    bne queue_stop
    mov r0, #MIC_PXI_NOTICE_BUFFER_FULL
    mov r1, #MIC_PXI_RESULT_SUCCESS
    bl SPIi_ReturnResult
    b done
queue_stop:
    mov r0, #SERIAL_OWNER_MICROPHONE
    mov r1, #MIC_PXI_COMMAND_AUTO_STOP
    mov r2, #0
    bl SPIi_SetEntry
    cmp r0, #0
    bne stop_timer
    mov r0, #MIC_PXI_NOTICE_BUFFER_FULL
    mov r1, #MIC_PXI_RESULT_EXCLUSIVE
    bl SPIi_ReturnResult
    b done
stop_timer:
    mov r0, #MIC_STATUS_END_WAIT
    str r0, [r4, #0x20]
    ldr r1, =0x0400010e
    ldrh r0, [r1]
    bic r0, r0, #TIMER_ENABLE
    strh r0, [r1]
done:
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
}
#else
void func_arm7_03805904(void) {
  MICWork* work = &data_arm7_0380af74;
  u16 mode = work->mode;
  u16 pending = work->pendingSample;
  u16 sample;
  u16 pair;
  u16* at;
  u32 offset;

  if ((mode & MIC_TYPE_NO_REPEAT) != MIC_TYPE_NO_REPEAT) {
    sample = work->lastSample;
  } else {
    sample = 0xffff;
  }
  if (!SPIi_CheckEntry() && SPIi_CheckException(SERIAL_OWNER_MICROPHONE)) {
    if ((mode & MIC_TYPE_12BIT) == MIC_TYPE_12BIT) {
      sample = func_arm7_038060bc();
      if (mode & MIC_TYPE_SIGNED) {
        sample ^= 0x8000;
      }
    } else {
      sample = func_arm7_03806184();
      if (mode & MIC_TYPE_SIGNED) {
        sample ^= 0x80;
      }
    }
  }

  offset = work->offset;
  if ((mode & MIC_TYPE_12BIT) == MIC_TYPE_12BIT) {
    at = (u16*)(work->buffer + offset);
    *at = sample;
    MIC_SYSTEM_WORK->lastAddress = at;
    MIC_SYSTEM_WORK->lastSample = sample;
    offset += 2;
  } else {
    sample &= 0xff;
    if ((offset & 1) == 0) {
      pending = sample;
      offset += 1;
    } else {
      pair = (u16)(pending | sample << 8);
      offset -= 1;
      at = (u16*)(work->buffer + offset);
      *at = pair;
      MIC_SYSTEM_WORK->lastAddress = at;
      MIC_SYSTEM_WORK->lastSample = pair;
      offset += 2;
    }
  }
  work->pendingSample = pending;
  work->lastSample = sample;

  if (offset < work->size) {
    work->offset = offset;
    return;
  }
  work->offset = 0;
  if ((work->type & MIC_TYPE_LOOP) == MIC_TYPE_LOOP) {
    SPIi_ReturnResult(MIC_PXI_NOTICE_BUFFER_FULL, MIC_PXI_RESULT_SUCCESS);
  } else if (SPIi_SetEntry(SERIAL_OWNER_MICROPHONE, MIC_PXI_COMMAND_AUTO_STOP,
                           0)) {
    work->status = MIC_STATUS_END_WAIT;
    REG_TM3CNT_H &= ~TIMER_ENABLE;
  } else {
    SPIi_ReturnResult(MIC_PXI_NOTICE_BUFFER_FULL, MIC_PXI_RESULT_EXCLUSIVE);
  }
}
#endif
