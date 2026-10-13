#ifndef GAME_SCREEN_CONTEXTS_H
#define GAME_SCREEN_CONTEXTS_H

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/oam_buffer.h"
#include "game/task.h"
#include "game/weather.h"

/* Storage that Screens_InitContexts hands to the two screen contexts. A
 * pair of arrays or variables holds the main screen's first, then the sub
 * screen's; a 2-element array holds both, main screen first. */
extern u8 data_0216e5ac[2][2]; /* Unk_02080440.unk_58 */
extern u16 data_0216e5b0[2];
extern s16 data_0216e5b4[2];
extern u8 data_0216e5b8[4]; /* Unk_02080440.unk_28 */
extern u8 data_0216e5bc[4];
extern Unk_02080440_5c data_0216e5c0[2];
extern struct Task* data_0216e5d4[TASK_LEVELS];
extern struct Task* data_0216e5f4[TASK_LEVELS];
extern u8 data_0216e614[0x48]; /* Unk_027c0280.textPrinter */
extern u8 data_0216e65c[0x48];
extern Unk_02080440_50 data_0216e6a4;
extern Unk_02080440_50 data_0216e708;
extern SpriteEntry data_0216e76c[16]; /* sprite list heads */
extern SpriteEntry data_0216e86c[16];
extern struct Task* data_0216e96c[TASK_COUNT + 1];
extern struct Task* data_0216eab0[TASK_COUNT + 1];
extern struct WeatherFx data_0216ebf4;
extern struct WeatherFx data_0216edc4;
extern struct FontState data_0216f5f4[2];
extern u8 data_0216fca8[0x400]; /* main screen palettes */
extern struct OamBuffer data_021704a8;
extern struct OamBuffer data_02170ad8;
extern SpriteEntry data_02171108[128]; /* sprite pools */
extern SpriteEntry data_02171908[128];
extern u8 data_02177b08[0x1000]; /* Unk_027c0280.unk_04 */
extern u8 data_0217db08[0x1000];

#endif
