#ifndef GAME_SCREEN_CONTEXTS_H
#define GAME_SCREEN_CONTEXTS_H

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"
#include "game/oam_buffer.h"
#include "game/script_slot.h"
#include "game/task.h"
#include "game/weather.h"

/* Storage that Screens_InitContexts hands to the two screen contexts. A
 * pair of arrays or variables holds the main screen's first, then the sub
 * screen's; a 2-element array holds both, main screen first. */
extern u8 data_0216e5ac[][2]; /* Unk_02080440.unk_58 */
extern u16 data_0216e5b0[];
extern s16 data_0216e5b4[];
extern u8 data_0216e5b8[]; /* Unk_02080440.unk_28 */
extern u8 data_0216e5bc[];
extern Unk_02080440_5c data_0216e5c0[];
extern struct Task* data_0216e5d4[TASK_LEVELS];
extern struct Task* data_0216e5f4[TASK_LEVELS];
extern u8 data_0216e614[]; /* Unk_027c0280.textPrinter */
extern u8 data_0216e65c[];
extern Unk_02080440_50 data_0216e6a4;
extern Unk_02080440_50 data_0216e708;
extern SpriteEntry data_0216e76c[]; /* sprite list heads */
extern SpriteEntry data_0216e86c[];
extern struct Task* data_0216e96c[TASK_COUNT + 1];
extern struct Task* data_0216eab0[TASK_COUNT + 1];
extern struct WeatherFx data_0216ebf4;
extern struct WeatherFx data_0216edc4;
extern IconBank data_0216ef94[ICON_BANK_COUNT]; /* Unk_027c0280.iconBanks */
extern IconBank data_0216f2c4[ICON_BANK_COUNT];
extern struct FontState data_0216f5f4[];
extern u8 data_0216fca8[]; /* main screen palettes */
extern u8 data_021700a8[]; /* sub screen palettes */
extern struct OamBuffer data_021704a8;
extern struct OamBuffer data_02170ad8;
extern SpriteEntry data_02171108[]; /* sprite pools */
extern SpriteEntry data_02171908[];
extern ScriptSlot data_02172108[SCRIPT_SLOT_COUNT];
extern ScriptSlot data_02172c48[SCRIPT_SLOT_COUNT];
extern struct Task data_02173788[TASK_COUNT]; /* task pools */
extern struct Task data_02175948[TASK_COUNT];
/* Layer map buffers, layers 0-3; layer 0's is also Unk_027c0280.unk_04. */
extern u8 data_02177b08[]; /* Unk_027c0280.unk_04 */
extern u8 data_02178b08[GFX_LAYER_MAP_SIZE];
extern u8 data_02179b08[GFX_LAYER_MAP_SIZE];
extern u8 data_0217ab08[GFX_LAYER3_MAP_SIZE];
extern u8 data_0217db08[];
extern u8 data_0217eb08[GFX_LAYER_MAP_SIZE];
extern u8 data_0217fb08[GFX_LAYER_MAP_SIZE];
extern u8 data_02180b08[GFX_LAYER3_MAP_SIZE];

#endif
