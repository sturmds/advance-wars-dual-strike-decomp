#include "types.h"
#include "game/screen_other_map.h"
#include "game/affine_params.h"

/* Byte for byte the same code as Screens_SetBg3Zoom, at another address:
 * sets the BG3 affine matrix of both screens to one scale and angle. */
void TitleCall_SetBg3Zoom(int scale, int angle)
{
    s16 scale16;
    AffineParams params;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    params.srcX = 256;
    scale16 = scale; /* needed to match: assigned between the two 256 stores */
    params.srcY = 256;
    params.scaleX = scale16;
    params.scaleY = scale16;
    params.angle = angle;
    params.centerX = 128;

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    params.centerY = 0;
    BgAffine_FromParams(&params, &data_027c0280->bg3Affine);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    params.centerY = 192;
    BgAffine_FromParams(&params, &data_027c0280->bg3Affine);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
