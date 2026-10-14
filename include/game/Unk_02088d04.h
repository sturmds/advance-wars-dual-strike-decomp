#ifndef GAME_UNK_02088D04_H
#define GAME_UNK_02088D04_H

#include "types.h"
#include "game/BgControl.h"

/* One sprite queued for drawing. Each sprite list starts with an entry
 * of this type whose next field points to the newest sprite. */
typedef struct SpriteEntry {
  struct SpriteEntry* next;
  s16 x; /* OAM attribute 1 bits: x and flip flags */
  s16 y; /* OAM attribute 0 bits: y and above */
  u16 unk_08;
  u16 unk_0a;
  const void* unk_0c;
} SpriteEntry;

/* DISPCNT bits 0-15 as Unk_027c0280.dispCnt keeps them. dispCnt is a
 * plain u16 for the code that sets bits; GFX_DISPCNT reads it as this. */
typedef struct DispCntBits {
  u16 bgMode : 3;
  u16 : 2;
  u16 objHBlankProc : 1; /* DISPCNT bit 23 */
  u16 objMap1D : 1;      /* DISPCNT bit 4 */
  u16 displayOff
      : 1; /* DISPCNT bit 7 (forced blank); set: Screen_ApplyMain calls GX_DispOff, Screen_ApplySub clears DISPCNT bit 16 */
  u16 bg0 : 1; /* bits 8-12: layers shown */
  u16 bg1 : 1;
  u16 bg2 : 1;
  u16 bg3 : 1;
  u16 obj : 1;
  u16 win0 : 1; /* bits 13-15: windows on */
  u16 win1 : 1;
  u16 objWin : 1;
} DispCntBits;

#define GFX_BG_MODE(gfx) (((DispCntBits*)&(gfx)->dispCnt)->bgMode)

#define GFX_DISPCNT(gfx) ((DispCntBits*)&(gfx)->dispCnt)

/* Unk_027c0280.blendMode value: darken by blendEvy / 16 (BLDCNT mode 3). */
#define BLEND_MODE_DARKEN 3

/* Unk_027c0280.blendMode value: brighten by blendEvy / 16 (BLDCNT mode 2). */
#define BLEND_MODE_BRIGHTEN 2

/* Unk_027c0280.blendMode value: alpha blend with blendEva and blendEvb (BLDCNT mode 1). */
#define BLEND_MODE_ALPHA 1

/* Unk_027c0280.blendMode value: no colour effect (BLDCNT mode 0). */
#define BLEND_MODE_NONE 0

/* Layers inside one window (WININ/WINOUT half). */
typedef struct WindowPlanes {
  u8 bg[4];
  u16 obj : 1;
  u16 effect : 1; /* colour effects apply inside */
  u16 : 14;
} WindowPlanes;

/* BG2 or BG3 rotation and scaling registers. */
typedef struct BgAffine {
  s16 pa;
  s16 pb;
  s16 pc;
  s16 pd;
  s32 x;
  s32 y;
} BgAffine;

/* One string drawn by FontCache_Add. */
typedef struct FontCacheEntry {
  u16 tile;     /* first BG tile of the drawn string */
  u8 halfTiles; /* tiles used, halved */
  u8 colour;
} FontCacheEntry;

/* Pointed to by Unk_027c0280.fontState. Only two fields are known. */
typedef struct FontState {
  u8 unk_00; /* set to 1 when digitEndTile changes */
  u8 pad_01[0x258 - 0x01];
  FontCacheEntry cache
      [64]; /* strings drawn into BG tiles, in the order of the names from 0x00 */
  u16 digitEndTile; /* tile after the digit font (Font_LoadDigits) */
} FontState;

/* Pointed to by data_027c0280 (DTCM). */
typedef struct Unk_027c0280 {
  struct OamBuffer* oamBuffer; /* sprite entries waiting to be uploaded */
  void* unk_04;
  u8* paletteBuffer; /* BG and OBJ palettes, 0x400 bytes; Palette_Load writes, Palette_Flush uploads */
  u32 paletteUploadQueued; /* set by Palette_Flush when the upload is queued, until it runs */
  u32 layerCopyMask; /* bit per layer: copy its tile map to VRAM (Gfx_CopyRequestedLayers) */
  struct FontState* fontState;
  void*
      textPrinter; /* text, position and target map of the text being printed */
  struct IconBank* iconBanks; /* ICON_BANK_COUNT banks */
  SpriteEntry* spritePool;    /* spriteFree starts here (Sprites_ResetLists) */
  SpriteEntry* spriteLists;
  SpriteEntry* spriteFree; /* next unused entry */
  u8 pad_2c[0x6a - 0x2c];
  u16 : 3;
  u16 vBlankIrq : 1; /* DISPSTAT bits 3-5 */
  u16 hBlankIrq : 1;
  u16 vCountIrq : 1;
  u16 : 2;
  u16 vCountMatch : 8; /* DISPSTAT bits 8-15 */
  u16 dispCnt; /* DISPCNT bits 0-15 (bit 8-12: BG0-3 and OBJ on); DispCntBits names the bits */
  BgControl bgCnt[4]; /* BG0CNT-BG3CNT */
  u16 bgMosaicH : 4;  /* MOSAIC */
  u16 bgMosaicV : 4;
  u16 objMosaicH : 4;
  u16 objMosaicV : 4;
  WindowPlanes win0In;   /* WININ bits 0-5 */
  WindowPlanes win1In;   /* WININ bits 8-13 */
  WindowPlanes winOut;   /* WINOUT bits 0-5 */
  WindowPlanes objWinIn; /* WINOUT bits 8-13 */
  u8 blendTopBg[4];      /* BLDCNT first target, per BG */
  u16 blendTopObj : 1;
  u16 blendTopBackdrop : 1;
  u16 blendMode : 2; /* 0 none, 1 alpha */
  u16 : 12;
  u8 blendBottomBg[4]; /* BLDCNT second target, per BG */
  u16 blendBottomObj : 1;
  u16 blendBottomBackdrop : 1;
  u16 : 14;
  BgAffine bg2Affine;
  BgAffine bg3Affine;
  u16 blendEva;
  u16 blendEvb;
  u16 blendEvy;
  u16 bgScrollX[4];
  u16 bgScrollY[4];
  u8 win0Left; /* WIN0H, WIN0V */
  u8 win0Right;
  u8 win0Top;
  u8 win0Bottom;
  u8 win1Left; /* WIN1H, WIN1V */
  u8 win1Right;
  u8 win1Top;
  u8 win1Bottom;
  u8 dispMode;       /* GX_SetGraphicsMode arguments */
  u8 bg0Mode;        /* 1: BG0 shows the 3D engine */
  u8 bgScreenOffset; /* DISPCNT bits 27-29 */
  u8 unk_dd;
  s8 masterBrightness; /* -16 (black) to 16 (white) */
  u8 pad_df[0xe0 - 0xdf];
  u8 layer0Mode; /* GfxLayer.mode of layer 0 */
  /* Four layers of 0x14 bytes from 0xe0: +1 the BG they use, +0xc
     * their tile map buffer. */
  u8 layer0Bg;
  u8 pad_e2[0xe4 - 0xe2];
  u8* unk_e4;   /* tile data that the digit font loads into */
  void* unk_e8; /* where TransferQueue_Add copies layer0Map to (VRAM) */
  u16* layer0Map;
  u8 pad_f0[0xf5 - 0xf0];
  u8 layer1Bg;
  u8 pad_f6[0xf8 - 0xf6];
  void* unk_f8;
  u8 pad_fc[0x100 - 0xfc];
  u16* layer1Map;
  u8 pad_104[0x109 - 0x104];
  u8 layer2Bg;
  u8 pad_10a[0x10c - 0x10a];
  void*
      unk_10c; /* layer 2 tile data; the win/lose screen copies the main screen's to the sub screen */
  u8 pad_110[0x114 - 0x110];
  u16* layer2Map;
  u8 pad_118[0x11d - 0x118];
  u8 layer3Bg;
  u8 pad_11e[0x120 - 0x11e];
  u8* unk_120; /* layer 3 tile data, like unk_e4 for layer 0 */
  u8 pad_124[0x128 - 0x124];
  u16* layer3Map;
  u8 pad_12c[0x130 - 0x12c];
  u8* bgVram;  /* BG character base 0 */
  u8* objVram; /* CO pictures load here, 32 bytes per tile */
  u8* paletteCopy; /* 0x400 bytes; Screen_ResetAndShowMessage copies paletteBuffer here */
  u8* objPaletteVram; /* OBJ palette RAM of the screen's engine */
  u8* oamDest;        /* the OAM copy that OamBuffer entries are uploaded to */
} Unk_027c0280;

/* data_027c0288 (DTCM). */
/* One of the four layers at Unk_027c0280.layer0Mode, 0x14 bytes. */
typedef struct GfxLayer {
  u8 mode; /* GFX_LAYER_NO_COPY: map copies and clears skip this layer */
  u8 bg;
  u8 pad_02[0x04 - 0x02];
  u32 charBase;   /* byte offset into BG VRAM */
  u32 screenBase; /* byte offset into BG VRAM */
  u16* map;
  u8 screenSize; /* BGxCNT screen size */
  u8 pad_11[0x14 - 0x11];
} GfxLayer;

typedef char GfxLayer_size_check[sizeof(GfxLayer) == 0x14 ? 1 : -1];

/* Bytes of a screen's BG and OBJ palette buffer (Unk_027c0280.paletteBuffer). */
#define PALETTE_BUFFER_SIZE 0x400

/* Bytes in the map buffer (GfxLayer.map) of layers 0-2 of a screen, and of
 * layer 3. Screens_InitContexts gives each screen four, in layer order
 * (screen_contexts.h). */
#define GFX_LAYER_MAP_SIZE 0x1000
#define GFX_LAYER3_MAP_SIZE 0x3000
/* Entries (u16) in GFX_LAYER_MAP_SIZE bytes of a layer map. */
#define GFX_LAYER_MAP_ENTRIES (GFX_LAYER_MAP_SIZE / 2)

/* One BG screen block: a map of 32 x 32 entries (u16), 0x800 bytes. The
 * display shows its first 24 rows. */
#define BG_SCREEN_ENTRIES (32 * 32)
#define BG_SCREEN_SIZE (BG_SCREEN_ENTRIES * 2)
#define BG_VISIBLE_ENTRIES (32 * 24)
#define BG_VISIBLE_SIZE (BG_VISIBLE_ENTRIES * 2)

/* Bytes of one 16-colour palette (BGR555), in a palette buffer or VRAM. */
#define PALETTE_16_SIZE 0x20
#define PALETTE_16_COLOURS 16

/* 16-colour BG palettes at the start of a palette buffer; the OBJ palettes
 * follow. */
#define BG_PALETTE_COUNT 16

/* Pixels of a screen, and of the side of an 8x8 tile. */
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define TILE_PIXELS 8

/* Bytes of one 8x8 tile at 4 bits per pixel. */
#define TILE_4BPP_SIZE 0x20

/* Bytes of one block of the tables Palette_GetBlock and Palette_GetArmyBlock
 * return from. */
#define PALETTE_BLOCK_SIZE 256

/* GfxLayer.mode */
#define GFX_LAYER_NO_COPY 2

/* Layer i (0-3) of an Unk_027c0280. */
#define GFX_LAYER(gfx, i) (&((GfxLayer*)&(gfx)->layer0Mode)[i])

/* Unk_027c0280 seen as its layer array; reaching a layer this way adds the
 * layer's offset before the field's, as some ITCM code does. */
/* Layers of a screen (GFX_LAYER). */
#define GFX_LAYER_COUNT 4

typedef struct GfxLayerView {
  u8 pad_00[0xe0];
  GfxLayer layer[GFX_LAYER_COUNT];
} GfxLayerView;

#define GFX_LAYERS(gfx) (((GfxLayerView*)(gfx))->layer)

typedef struct Unk_027c0288 {
  void* unk_00;
  void* unk_04; /* loaded into data_027c027c and data_027c0280 */
} Unk_027c0288;

/* A 0x6c-byte object found by Task_FindByScript. */
typedef struct Unk_02023724 {
  u8 pad_00[0x32];
  u8 unk_32; /* selected entry of a list */
} Unk_02023724;

#endif
