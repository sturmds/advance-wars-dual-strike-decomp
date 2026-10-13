#include "types.h"
#include "game/Unk_0216e2b0.h"
#include "game/callback.h"
#include "game/rng.h"
#include "game/screen_shake.h"
#include "game/screen_wipe.h"
#include "game/window_wipe.h"

/* eased value; ScreenWipe_EaseDownUpdate takes it from 352 to 0 */
s16 data_0216e29c;
s16 data_0216e2a8; /* wipe progress, from 0 up to the wipe's end */
volatile s16 gScreenShakeX;
volatile s16 gCallbackCount;
volatile s16 gScreenShakeY;
u32 gRngState;
Unk_0216e2b0 data_0216e2b0;
void (*gCallbackList[CALLBACK_SLOTS])(void);
void (*volatile gCallbackSlots[CALLBACK_SLOTS])(void);
u16 gWipeWin0hTable[WIPE_LINES]; /* WIN0H per scanline: left << 8 | right */
