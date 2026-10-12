#ifndef SDK_WL_H
#define SDK_WL_H

#include "types.h"

// WL is the ARM7 wireless driver below WM. Its work area address is kept at
// 0x0380fff4. The request handlers get a request block and fill a reply block.

typedef struct WlRequest {
    u8 pad_00[0x10];
    u16 unk_10;
    u16 unk_12;
} WlRequest;

typedef struct WlReply {
    u16 unk_00;
    u16 status;
    u16 unk_04;
    u16 value;
    u16 words[16];
} WlReply;

typedef struct WlLimits {
    u8 pad_00[0xf4];
    u16 unk_f4;
    u16 unk_f6;
} WlLimits;

typedef struct WlState {
    u16 unk_00;
    u16 phase;
    u8 pad_04[0x1c];
    u16 mode;
    u8 pad_22[0x8];
    u16 unk_2a;
    u8 pad_2c[0xa];
    u16 unk_36;
    u16 words[16];
    u8 pad_58[0x2e];
    u16 unk_86;
    u16 unk_88;
    u8 pad_8a[2];
    u16 unk_8c;
} WlState;

typedef struct WlWork {
    u8 pad_000[0x200];
    WlLimits limits;
    u8 pad_2f8[8];
    WlState state;
} WlWork;

#define WIFI_DRIVER_STATE (*(WlWork **)0x0380fff4)

#endif
