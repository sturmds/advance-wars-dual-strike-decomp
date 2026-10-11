#include "types.h"
#include "game/screen_other_map.h"
#include "game/affine_params.h"

void func_020523e0(int scale, int angle)
{
    s16 scale16;
    AffineParams params;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    params.srcX = 256;
    scale16 = scale;  
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
