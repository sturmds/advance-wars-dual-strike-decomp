#ifndef SDK_CARD_H
#define SDK_CARD_H

#include "types.h"

/* Game card registers. */
#define IO_GAMECARD_MASTER (*(volatile u8 *)0x040001a1)
#define IO_GAMECARD_ROM_CTRL (*(volatile u32 *)0x040001a4)
#define IO_GAMECARD_CMD_BYTES ((volatile u8 *)0x040001a8)   /* 8 command bytes, sent first byte first */
#define IO_GAMECARD_READ_PORT (*(volatile u32 *)0x04100010)

/* MASTERCNT: card enable, with interrupts off and ROM (not backup) mode. */
#define CARDMST_ENABLE 0x80

/* GAMECARD_ROM_CTRL bits. */
#define CARD_DATA_READY 0x00800000
#define CARD_COMMAND_MASK 0x07000000   /* block size field */
#define GAMECARD_XFER_ONE_PAGE 0x01000000         /* one 512-byte block */
#define CARD_RESET_HI 0x20000000
#define CARD_START 0x80000000

/* Card command: read one 512-byte page. */
#define CARD_CMD_READ_PAGE 0xb7

#define GAMECARD_ROM_PAGE_BYTES 512

#endif
