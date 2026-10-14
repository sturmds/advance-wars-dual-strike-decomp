#include "types.h"

/* "0b0" */
char gWeatherSnowFileName[4] = {
#include "data/main/gWeatherSnowFileName.inc.c"
};
/* "0b1" */
char gWeatherRainFileName[4] = {
#include "data/main/gWeatherRainFileName.inc.c"
};
/* "0b2" */
char gWeatherSandFileName[4] = {
#include "data/main/gWeatherSandFileName.inc.c"
};
/* "/bmap" */
char gWeatherCloudDirName[6] = {
#include "data/main/gWeatherCloudDirName.inc.c"
};
