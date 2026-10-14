#include "types.h"
#include "game/main_menu.h"
#include "game/unlocks.h"
#include "game/play_records.h"

void Gx_DrawTexturedQuadFormat(int x, int y, int a2, int a3, int a4, int a5,
                               int a6, int a7, int a8, int a9);
int Score_ToRank(int score); /* 2 below 200, 3 below 250, 4 below 280, else 5 */
void MainMenu_DrawRankIcon(int x, int y, int icon);

/* Draws the current main menu page as 3D quads: the title on top, two rows
 * of two quads per entry 24 pixels apart (hidden entries use palette + 9),
 * then the bottom edge. On page 11, entry kinds 25 and 26 also get a rank
 * icon once game flag 33 or 34 is set, from the best campaign averages (gPlayRecords.campaignBestAverage). */
void MainMenu_DrawPage(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entry = page->entries;
  int y = -((page->entryCount + 1) * 3)
          << 2; /* -(count + 1) * 12; the shift form is needed to match */
  int i;

  Gx_DrawTexturedQuadFormat(-96, y, 0, 4, 1, 3, page->title * 0x600,
                            page->palette * 32, 0, 0);
  Gx_DrawTexturedQuadFormat(32, y, 0, 3, 1, 3, (page->title * 48 + 32) * 32,
                            page->palette * 32, 0, 0);
  y += 16;
  for (i = 0; i < page->entryCount; i++, entry++) {
    Gx_DrawTexturedQuadFormat(-96, y, 0, 4, 1, 3, (entry->kind * 72 + 456) * 32,
                              (entry->hidden * 9 + entry->palette) * 32, 0, 0);
    Gx_DrawTexturedQuadFormat(32, y, 0, 3, 1, 3, (entry->kind * 72 + 488) * 32,
                              (entry->hidden * 9 + entry->palette) * 32, 0, 0);
    Gx_DrawTexturedQuadFormat(-96, y + 16, 0, 4, 0, 3,
                              (entry->kind * 72 + 504) * 32,
                              (entry->hidden * 9 + entry->palette) * 32, 0, 0);
    Gx_DrawTexturedQuadFormat(32, y + 16, 0, 3, 0, 3,
                              (entry->kind * 72 + 520) * 32,
                              (entry->hidden * 9 + entry->palette) * 32, 0, 0);
    if (data_021599d0->page == 11) {
      switch (entry->kind) {
        case 25:
          if (GameFlag_IsSet(33)) {
            MainMenu_DrawRankIcon(
                67, y + 2,
                5 - Score_ToRank(gPlayRecords.campaignBestAverage[0]));
          }
          break;
        case 26:
          if (GameFlag_IsSet(34)) {
            MainMenu_DrawRankIcon(
                67, y + 2,
                5 - Score_ToRank(gPlayRecords.campaignBestAverage[1]));
          }
          break;
      }
    }
    y += 24;
  }
  Gx_DrawTexturedQuadFormat(-96, y, 0, 4, 0, 3, 0x3600, page->palette * 32, 0,
                            0);
  Gx_DrawTexturedQuadFormat(32, y, 0, 3, 0, 3, 0x3800, page->palette * 32, 0,
                            0);
}
