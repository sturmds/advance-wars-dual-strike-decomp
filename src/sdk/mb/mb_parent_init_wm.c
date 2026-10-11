#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

extern DlPlayBufferArea *data_022aced4;
extern DlPlayStationArea *data_022aced8;
extern void *data_022acedc;
extern u16 data_022aced0;

void func_0211bfb8(int);
int WM_Initialize(void *workArea, void (*callback)(void), u16 dmaNo);
void func_0211d8a8(void);

/* Clears the parent's progress, then brings WM up (WM_Initialize, WM_Initialize)
 * unless the game started Download Play earlier, and installs the indication
 * callback. Returns 0, or 8 when WM_Initialize fails. */
int MbParent_InitWm(void)
{
    data_022aced8->unk_528 = 0;
    data_022aced8->unk_52a = 0;
    data_022aced8->unk_526 = 0;
    data_022aced8->unk_548 = 0;
    func_0211bfb8(10);
    if (data_022aced4->parent.unk_320 == 0) {
        int result;

        do {
            result = WM_Initialize(data_022acedc, data_022aced8->sendCallback, data_022aced0);
        } while (result == WM_ERRCODE_WM_DISABLE);
        if (result != WM_ERRCODE_OPERATING) {
            return 8;
        }
        WM_SetIndCallback(data_022aced8->sendCallback);
        data_022aced8->unk_50d = 1;
        return 0;
    }
    WM_SetIndCallback(data_022aced8->sendCallback);
    data_022aced8->unk_50d = 1;
    func_0211d8a8();
    return 0;
}
