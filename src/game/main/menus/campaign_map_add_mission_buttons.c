#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_button.h"

int CampaignMap_CenterScrollX(int x);
int func_0204ec34(int y);

extern CampaignMapState gCampaignMapState;
extern CampaignMission gCampaignMissions[];
extern MenuButtonTemplate data_0215b3d0;
extern MenuButtonTemplate data_0215b3f8;
extern MenuButtonTemplate data_0215b448;

#define MISSION gCampaignMissions[gCampaignMapState.mission]

/* Adds the button over the current mission (id 252) at its screen position,
 * left of the mission, or 80 pixels further left when the mission is in the
 * right half of the screen, and two buttons off screen (253, 254), and
 * selects 253. */
void CampaignMap_AddMissionButtons(void) {
  int buttonX;
  int buttonY;

  buttonX = MISSION.x - CampaignMap_CenterScrollX(MISSION.x);
  buttonY = MISSION.y - func_0204ec34(MISSION.y);
  if (MISSION.x - CampaignMap_CenterScrollX(MISSION.x) > 120) {
    buttonX -= 80;
  } else {
    buttonX -= 8;
  }
  MenuButton_Add(buttonX, buttonY - 8, &data_0215b448, 252);
  MenuButton_Add(0, 0, &data_0215b3d0, 253);
  MenuButton_Add(0, 0, &data_0215b3f8, 254);
  MenuButton_Select(253);
}
