#include "types.h"

void MenuDepth_Push(void);
void ScreenTransition_IsRunning(void);
void WeatherTask_Apply(void);
void MenuDepth_Pop(void);
void WeatherTask_PlaySound(void);
void func_020c4018(void);

/* palette file in /bmap */
char data_02166cdc[4] = {
#include "data/main/data_02166cdc.inc.c"
};
u16 gWeatherSandSprite[4] = {
#include "data/main/gWeatherSandSprite.inc.c"
};
u16 gWeatherRainSprites[3][4] = {
#include "data/main/gWeatherRainSprites.inc.c"
};
u16 gWeatherSnowSprites[3][4] = {
#include "data/main/gWeatherSnowSprites.inc.c"
};
u32 data_02166d18[24] = {
#include "data/main/data_02166d18.inc.c"
};
