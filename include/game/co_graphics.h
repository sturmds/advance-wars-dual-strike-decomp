#ifndef GAME_CO_GRAPHICS_H
#define GAME_CO_GRAPHICS_H

#include "types.h"
#include "game/co_data.h"

/* One picture variant of a CO: file names under /syogun. */
typedef struct CoPictureFiles {
  const char* name; /* loaded by Co_LoadGraphics */
  const char* unk_04;
} CoPictureFiles;

#define CO_PALETTE_COLOURS 16 /* colours in one of CoGraphics.palettes */

/* Rows of gCoGraphics after the COs: a soldier of each army, a face only,
 * which dialogue shows (event scripts). Co_GetPictureVariant and the other
 * variant lookups give them variant 0, as for any id from CO_COUNT on. */
enum CoSoldier {
  CO_SOLDIER_ORANGE_STAR = CO_COUNT,
  CO_SOLDIER_BLUE_MOON,
  CO_SOLDIER_GREEN_EARTH,
  CO_SOLDIER_YELLOW_COMET,
  CO_SOLDIER_BLACK_HOLE,
  CO_GRAPHICS_ROWS
};

/* Event scripts number the soldiers from 28, their first row in retail, and
 * keep that number when CO_COUNT grows. */
#define EVENT_SOLDIER_FIRST 28

/* A picture from an event script (CO id, or a soldier from
 * EVENT_SOLDIER_FIRST; mood in bits 8-9) as the id the CO picture loaders
 * take. The same number in retail, where CO_SOLDIER_ORANGE_STAR is 28. */
#define EVENT_PICTURE_TO_CO_MOOD(picture)                          \
  ((picture) + (((picture) & CO_ID_MASK) >= EVENT_SOLDIER_FIRST    \
                    ? CO_SOLDIER_ORANGE_STAR - EVENT_SOLDIER_FIRST \
                    : 0))

/* One entry of gCoGraphics (main), indexed by CO id or CoSoldier. */
typedef struct CoGraphics {
  CoPictureFiles pictures[10]; /* indexed by the CO's stored picture variant */
  const u8* palettes; /* palettes of CO_PALETTE_COLOURS BGR555 colours */
} CoGraphics;

extern CoGraphics gCoGraphics[CO_GRAPHICS_ROWS];

/* Bytes of a CO's small picture (12 tiles, Co_LoadSmallPicture), medium
 * picture (18 tiles, Co_QueueArmyMediumPicture) and mood picture (36 tiles,
 * Co_QueueLargePictureCopy), at 4 bits per pixel. */
#define CO_SMALL_PICTURE_SIZE 0x180
#define CO_MEDIUM_PICTURE_SIZE 0x240
#define CO_MOOD_PICTURE_SIZE 0x480

/* Bytes of the face on a CO's map panel (CoPanel_Draw, PowerPanel*_Draw): 8
 * tiles at 4 bits per pixel. */
#define CO_PANEL_FACE_SIZE 0x100

/* The map panel faces, CO_NELL first (no row for CO_NONE): picture variant 0
 * and the alternate outfit. */
extern const u8 gCoPanelFaces[CO_COUNT - 1][CO_PANEL_FACE_SIZE];
extern const u8 gCoPanelFacesAlt[CO_COUNT - 1][CO_PANEL_FACE_SIZE];
#define CO_PANEL_FACE_ROW(coId) (((coId) & CO_ID_MASK) - 1)

/* Bytes of a CO's large picture, as Co_LoadArmyGraphicsToBuffer writes it. */
#define CO_LARGE_PICTURE_SIZE 0x3600

#endif
