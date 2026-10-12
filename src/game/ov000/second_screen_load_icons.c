#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"

extern Unk_027c0280* data_027c0280;

void IconBanks_ClearSlots(void);

/* Loads the icons the second screen draws: first into the tile data, then
 * into two object banks (0 and 3). */
void SecondScreen_LoadIcons(void) {
  Icon_LoadTo(28, data_027c0280->unk_e4, 576);
  Icon_LoadTo(29, data_027c0280->unk_e4, 584);
  Icon_LoadTo(30, data_027c0280->unk_e4, 592);
  Icon_LoadTo(31, data_027c0280->unk_e4, 600);
  Icon_LoadTo(33, data_027c0280->unk_e4, 608);
  Icon_LoadTo(32, data_027c0280->unk_e4, 616);
  Icon_LoadTo(34, data_027c0280->unk_e4, 624);
  Icon_LoadTo(35, data_027c0280->unk_e4, 632);
  Icon_LoadTo(36, data_027c0280->unk_e4, 640);
  Icon_LoadTo(48, data_027c0280->unk_e4, 648);
  Icon_LoadTo(49, data_027c0280->unk_e4, 654);
  Icon_LoadTo(50, data_027c0280->unk_e4, 660);
  Icon_LoadPalette(28, 12);
  IconBanks_ClearSlots();
  IconBank_Init(0, data_027c0280->objVram, 240, 17);
  Icon_Load(61);
  Icon_Load(6);
  Icon_Load(37);
  Icon_Load(38);
  Icon_Load(39);
  Icon_Load(40);
  Icon_Load(41);
  Icon_Load(43);
  Icon_Load(42);
  Icon_Load(45);
  Icon_Load(44);
  Icon_Load(46);
  Icon_Load(47);
  Icon_Load(63);
  IconBank_Init(3, data_027c0280->objVram, 340, 18);
  Icon_Load(90);
  Icon_Load(91);
  Icon_Load(92);
  Icon_Load(93);
  Icon_Load(94);
  Icon_Load(95);
  Icon_Load(96);
  Icon_Load(97);
  Icon_Load(98);
  Icon_Load(99);
  Icon_Load(100);
  Icon_Load(101);
}
