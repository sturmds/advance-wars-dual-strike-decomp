#ifndef GAME_CAPTURE_PICTURE_TASK_H
#define GAME_CAPTURE_PICTURE_TASK_H

#include "types.h"

struct SpriteAnim;

/* Work area of the task that draws a property while it is captured
 * (script data_02168df0: CapturePicture_Init sets it up, CapturePicture_Draw draws
 * it each frame). Capture_StartDisplay fills it. Its parent task (script
 * data_021690f8, CaptureTask_ShowCompletion) uses the same fields up to tile. */
typedef struct CapturePictureTask {
  u8 pad_00[0x2c];
  struct SpriteAnim*
      textSprite; /* the capture text (tile 404), affine matrix 1 */
  u8 pad_30[0x34 - 0x30];
  struct SpriteAnim*
      armySprite; /* the capturing army (CaptureArmyGfx_Load, tile 532), affine matrix 2 */
  struct SpriteAnim*
      propertySprite; /* the property (PropertyPicture_Load, tile 500), affine matrix 3, squashed as points go up */
  s16 x;              /* screen position of the picture */
  s16 y;
  s16 unk_40; /* CapturePicture_InitOffset: -(20 - oldPoints) * 2, the y offset CapturePicture_Draw gives armySprite */
  s16 unk_42; /* added to y for armySprite and propertySprite */
  u8 owner;   /* player capturing the property */
  u8 oldOwner; /* its owner before the capture */
  u16 points; /* capture points shown, 0-20; CapturePicture_Init starts it at oldPoints */
  u8 oldPoints; /* capture points before this capture action */
  u8 newPoints; /* capture points after it */
  u8 tile;      /* MapTiles.tiles byte of the property */
} CapturePictureTask;

/* Work area of the task CaptureTask_ShowCompletion starts when a capture completes
 * (script data_02168ea8); CaptureDone_Draw draws it. */
typedef struct CaptureDoneTask {
  u8 pad_00[0x30];
  struct SpriteAnim* sprite;
  u8 pad_34[0x3c - 0x34];
  s16 x; /* copied from CapturePictureTask.x */
  s16 y;
  u8 pad_40[0x4c - 0x40];
  s16 frame; /* counts up to 10 */
} CaptureDoneTask;

/* Work area of the tasks that count the capture points up or down (scripts
 * data_02168da8 and a sibling): each frame they step count and copy it to
 * the picture task's points. */
typedef struct CaptureCountTask {
  u8 pad_00[0x4c];
  s16 count; /* current step */
  s16 limit; /* newPoints, set by CaptureCountUp_Start */
} CaptureCountTask;

#endif
