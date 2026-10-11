#ifndef GAME_CAPTURE_PICTURE_TASK_H
#define GAME_CAPTURE_PICTURE_TASK_H

#include "types.h"

struct Unk_02022404;

/* Work area of the task that draws a property while it is captured
 * (func_020ec5cc sets it up, CapturePicture_Draw draws it each frame). */
typedef struct CapturePictureTask {
    u8 pad_00[0x2c];
    struct Unk_02022404 *picture;      /* the property (PropertyPicture_Load), affine matrix 1 */
    u8 pad_30[0x34 - 0x30];
    struct Unk_02022404 *unk_34;       /* drawn through affine matrix 2 */
    struct Unk_02022404 *unk_38;       /* drawn through affine matrix 3, squashed as points go up */
    s16 x;                             /* screen position of the picture */
    s16 y;
    u8 pad_40[0x42 - 0x40];
    s16 unk_42;                        /* added to y for the sprites in unk_34 and unk_38 */
    u8 unk_44;
    u8 pad_45;
    u16 points;                        /* capture points so far, 0-20; func_020ec5cc copies unk_48 here */
    u8 unk_48;
    u8 pad_49;
    u8 tile;                           /* MapTiles.tiles byte of the property */
} CapturePictureTask;

#endif
