#include "types.h"

BOOL Map_IsCoSelectPhase(void);
void PowerPanelLeft_DrawPlayer(int x, int y, int player, BOOL coSelect);
void PowerPanelRight_DrawPlayer(int x, int y, int player, BOOL coSelect);

/* Draws a player's power panel: the left design when x is below 120,
 * the right design otherwise. */
void PowerPanel_DrawPlayer(int x, int y, int player) {
  if (x < 120) {
    PowerPanelLeft_DrawPlayer(x, y, player, Map_IsCoSelectPhase());
  } else {
    PowerPanelRight_DrawPlayer(x, y, player, Map_IsCoSelectPhase());
  }
}
