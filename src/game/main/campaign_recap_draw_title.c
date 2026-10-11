#include "types.h"
#include "game/campaign_recap.h"
#include "game/menu_state.h"

int TitleCall_GetDigitWidth(int digit);
s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numerator, s32 denominator);
void Sprite_Queue(int list, int x, int y, void *data, int attr, int tile);

extern MenuState gMenuState;
extern u8 data_027c0040[];
extern u8 data_027c0048[];
extern u8 data_027c0148[];

#define TITLE ((CampaignRecapTitle *)&gMenuState)

/* Width in pixels of the label sprite before the digits. */
#define LABEL_WIDTH 106

/* Draws the first recap panel: the title sprite line from gMenuState, and
 * 20 pixels above it the label and the entry number (work->number) with the
 * title call digits. A two-digit number moves everything left by half the
 * tens digit width. Semi-transparent while the panel fades in. */
void CampaignRecap_DrawTitle(CampaignRecap *work)
{
    /* needed to match: this declaration order sets the stack slots */
    BOOL semiTransparent;
    int x;
    int lineY;
    int i;
    int number;
    int y;
    int tensWidth;
    int offset;

    number = work->number;
    tensWidth = TitleCall_GetDigitWidth(HwDiv32(number, 10)) + 1;
    semiTransparent = work->blend[0] != 16;

    if (work->blend[0] == 0) {
        return;
    }
    if (number >= 10) {
        offset = -(tensWidth >> 1);
    } else {
        offset = 0;
    }
    y = work->slide[0] + 48;
    for (i = 0; i < TITLE->spriteCount; i++) {
        Sprite_Queue(2, (offset + TITLE->spriteX[i]) & 0x1ff, (y & 0xff) | (semiTransparent << 10), data_027c0048,
                     0x1000, 64 + i * 16);
    }

    x = offset + 75;
    lineY = ((y - 20) & 0xff) | (semiTransparent << 10);
    Sprite_Queue(2, x & 0x1ff, lineY, data_027c0148, 0x400, 0);
    x += LABEL_WIDTH;
    if (number >= 10) {
        Sprite_Queue(2, x & 0x1ff, lineY, data_027c0040, 0x400, HwDiv32(number, 10) * 4 + 24);
        x += tensWidth;
    }
    Sprite_Queue(2, x & 0x1ff, lineY, data_027c0040, 0x400, HwMod32(number, 10) * 4 + 24);
}
