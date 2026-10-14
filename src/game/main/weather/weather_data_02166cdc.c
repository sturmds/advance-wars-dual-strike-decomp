#include "types.h"
#include "game/script_slot.h"

void MenuDepth_Push(void);
void ScreenTransition_IsRunning(void);
void WeatherTask_Apply(void);
void MenuDepth_Pop(void);
void WeatherTask_PlaySound(void);
void func_020c4018(void);

/* palette file in /bmap */
char data_02166cdc[] = {
#include "data/main/data_02166cdc.inc.c"
};
u16 gWeatherSandSprite[] = {
#include "data/main/gWeatherSandSprite.inc.c"
};
u16 gWeatherRainSprites[][4] = {
#include "data/main/gWeatherRainSprites.inc.c"
};
u16 gWeatherSnowSprites[][4] = {
#include "data/main/gWeatherSnowSprites.inc.c"
};
ScriptCommand data_02166d18[] = {
#include "data/main/data_02166d18.inc.c"
};
