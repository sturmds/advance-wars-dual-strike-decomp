#include "types.h"
#include "game/Unk_02080440.h"

void SidePanel_SlideAwayFromCursor(void);
void SidePanel_SnapLeft(void);
void SidePanel_SnapRight(void);
void SidePanel_SnapAwayFromCursor(void);

/* Calls the routine for one weather (WEATHER_*). */
void Weather_StartEffect(int weather)
{
    switch (weather) {
    case WEATHER_CLEAR:
        SidePanel_SlideAwayFromCursor();
        break;
    case WEATHER_SNOW:
        SidePanel_SnapLeft();
        break;
    case WEATHER_RAIN:
        SidePanel_SnapRight();
        break;
    case WEATHER_SAND:
        SidePanel_SnapAwayFromCursor();
        break;
    }
}
