#ifndef SDK_SVC_H
#define SDK_SVC_H

#include "types.h"

#define SHA1_CHUNK_BYTES 64
#define SHA1_HASH_BYTES 20

/* Results of the SHA-1 routines. */
#define SHA1_STATUS_GOOD 0
#define SHA1_STATUS_NULL_INPUT 1
#define SHA1_STATUS_TOO_LONG 1         /* more than 2^64 bits hashed */
#define SHA1_STATUS_ALREADY_FINAL 3                /* update after the hash was taken */

/* State of a running SHA-1 hash, 0x68 bytes. */
typedef struct SVCSHA1Context {
    u32 state[5];                      /* the five hash words */
    u32 bitsLow;                       /* bits hashed so far, low word */
    u32 bitsHigh;
    u32 used;                          /* bytes waiting in block */
    u8 block[SHA1_CHUNK_BYTES];
    u32 finished;                      /* set once the hash was padded and taken */
    u32 error;                         /* nonzero stops all further input */
} SVCSHA1Context;

/* BIOS system calls. */
void SVC_SoftReset(void);
void SVC_WaitByLoop(int count);
void Bios_AwaitIrqFlags(int clearFlags, u32 intrFlags);
void SVC_WaitVBlankIntr(void);
void SVC_Halt(void);
int SVC_Div(int numerator, int denominator);
int SVC_DivRem(int numerator, int denominator);
void SVC_CpuSet(const void *src, void *dst, u32 control);
void SVC_CpuFastSet(const void *src, void *dst, u32 control);
u32 SVC_Sqrt(u32 value);
u16 SVC_GetCRC16(u32 initial, const void *data, u32 size);
int SVC_IsDebugger(void);
void SVC_UnpackBits(const void *src, void *dst, const void *params);
void Bios_UnpackLz77ByBytes(const void *src, void *dst);
void Bios_UnpackLz77ByHalfwords(const void *src, void *dst);
void Bios_UnpackHuffman(const void *src, void *dst);
void Bios_UnpackRunLengthByBytes(const void *src, void *dst);
void Bios_UnpackRunLengthByHalfwords(const void *src, void *dst);

/* ARM7 BIOS system calls (the ARM7 build has its own copies). */
void Bios_RebootSoft_Arm7(void);
void Bios_DelayLoop_Arm7(int count);
void Bios_AwaitIrqFlags_Arm7(int clearFlags, u32 intrFlags);
void Bios_SleepUntilVBlank_Arm7(void);
void Bios_HaltUntilIrq_Arm7(void);
void Bios_EnterSleepMode_Arm7(void);
void Bios_RampSoundBias_Arm7(int raise, int delay);
void Bios_RampSoundBiasUp_Arm7(int delay);
void Bios_RampSoundBiasDown_Arm7(int delay);
int Bios_Quotient_Arm7(int numerator, int denominator);
int Bios_Remainder_Arm7(int numerator, int denominator);
void Bios_FillOrCopy_Arm7(const void *src, void *dst, u32 control);

int Sha1_ResetState(SVCSHA1Context *context);
int Sha1_FeedBytes(SVCSHA1Context *context, const u8 *data, u32 length);
int Sha1_FinishDigest(SVCSHA1Context *context, u8 *digest);

#endif
