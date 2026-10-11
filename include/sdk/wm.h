#ifndef SDK_WM_H
#define SDK_WM_H

#include "types.h"

/* What the callbacks passed to func_02117cac and its siblings receive. The
 * layout is the common WM callback header (apiid, errcode, ...); the wireless purpose is a guess from
 * that. errcode is 0 on success. */
typedef struct WMCallback {
    u16 apiid;
    u16 errcode;
} WMCallback;

#endif
