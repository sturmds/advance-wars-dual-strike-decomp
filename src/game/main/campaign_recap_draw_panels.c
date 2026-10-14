#include "types.h"
#include "game/campaign_recap.h"

extern u8 data_027c0030;
extern u8 data_027c0060[];
extern u8 data_027c01ac[];

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
int Score_ToRank_020e5428(int score);
void CampaignRecap_DrawNumber(CampaignRecap* work, int value, int x, int y,
                              int semiTransparent, int palette);

/* OBJ attribute 0 bit 10: semi-transparent (alpha blended). */
#define OBJ_SEMI_TRANSPARENT(on) ((on) << 10)

/* Draws the second and third recap panels (unk_20, then unk_30 with its
 * rank badge). A panel still fading in is drawn semi-transparent. */
void CampaignRecap_DrawPanels(CampaignRecap* work) {
  BOOL fading1 = work->blend[1] != 16;
  BOOL fading2 = work->blend[2] != 16;
  int slide1;
  int slide2;
  int y;

  if (work->blend[1] != 0) {
    slide1 = work->slide[1];
    y = ((slide1 + 80) & 0xff) | OBJ_SEMI_TRANSPARENT(fading1);
    Sprite_Queue(2, 72, y, &data_027c0060, 0x9000, 0x380);
    CampaignRecap_DrawNumber(work, work->unk_20, 114, slide1 + 80, fading1, 7);
    Sprite_Queue(2, 168, y, &data_027c0060, 0x9000, 0x388);
  }
  if (work->blend[2] == 0) {
    return;
  }
  slide2 = work->slide[2];
  y = ((slide2 + 136) & 0xff) | OBJ_SEMI_TRANSPARENT(fading2);
  Sprite_Queue(2, 16, y, &data_027c01ac, 0x8000, 0x2c4);
  CampaignRecap_DrawNumber(work, work->unk_30, 100, slide2 + 136, fading2, 8);
  Sprite_Queue(2, 144, y, &data_027c0060, 0x8000, 0x30c);
  /* Rank badge for the score: ranks 2-5 each have their own tiles. */
  switch (Score_ToRank_020e5428(work->unk_30)) {
    case 5:
      Sprite_Queue(2, 176,
                   ((slide2 + 112) & 0xff) | OBJ_SEMI_TRANSPARENT(fading2),
                   &data_027c0030, 0x2000, 0x180);
      break;
    case 4:
      Sprite_Queue(2, 176,
                   ((slide2 + 112) & 0xff) | OBJ_SEMI_TRANSPARENT(fading2),
                   &data_027c0030, 0x3000, 0x1c0);
      break;
    case 3:
      Sprite_Queue(2, 176,
                   ((slide2 + 112) & 0xff) | OBJ_SEMI_TRANSPARENT(fading2),
                   &data_027c0030, 0x4000, 0x200);
      break;
    case 2:
      Sprite_Queue(2, 176,
                   ((slide2 + 112) & 0xff) | OBJ_SEMI_TRANSPARENT(fading2),
                   &data_027c0030, 0x5000, 0x240);
      break;
  }
}
