# Map data

The campaign, Versus, War Room and Survival maps are not files in the ROM's file system. Their headers, tiles and pre-deployed units are data in overlay 0 (ov000), the overlay loaded during menus and battles. The map content is game data, so it never enters git (tenet 9). `make setup` unpacks it from your own ROM into one YAML file per map under `extracted/AWRE/assets/maps/` (gitignored), and `make` builds ov000's map tables from those files. Git holds only what says how to read and order them: the legend and the three glue files in `assets/maps/`. Combat mode's four maps are separate files (`/combat/%03x`) and are not covered here (issue 0050).

## Where the game reads them

| What | Where in the ROM | C |
|---|---|---|
| Header table, 0x10d entries of 0xa0 bytes, indexed by map id | ov000 `.data` 0x022dbd28-0x022e6548 | `Ov000_gMapHeaders`, `MapHeader` (`include/game/map_header.h`) |
| Tiles: one LZ77 block per map, plus 4 hard-mode blocks | ov000 `.rodata` 0x022b386c-0x022c9254 | `MapHeader.mapData`, `hardMapData`; `Map_LoadData` unpacks them into `MapData` |
| Pre-deployed units: 137 lists | ov000 `.data` 0x022e6548-0x022f4498 | `MapHeader.units`, `hardUnits`; `PredeployedUnit` (`include/game/predeployed_unit.h`) |
| Event scripts, Terms and Hint scripts | ov001 | `MapHeader.script`, `termsScript`, `hintScript`; kept as bytes, referenced by symbol |
| CO lists | main | `MapHeader.coList`; referenced by symbol |
| Map names | ov000 text banks (`Text_Get`) | `MapHeader.name` |

Map ids 0x101-0x10c are the Design Room slots. Their headers are in the table, but their tiles and units come from the save (`DesignMapRecord`).

A tile is a u16 code. A table loaded from `/bmap/003` turns the code into a terrain id (bits 0-4) and an owner (bits 5-7); the bottom map of Means to an End (0xf8) uses `/bmap/002` (`TileCodeTable_Load`). `docs/map-terrain.md` lists the codes of the large objects.

## Files

```
assets/maps/                    committed
  maps.txt        map id and YAML file, one line per entry of Ov000_gMapHeaders
  tiles.txt       YAML file and `tiles` or `hardTiles`, in the order the blocks sit in .rodata
  units.txt       YAML file and `units` or `hardUnits`, in the order the lists sit in .data
  legend.yaml     the token each tile code is written as (`o` for 0x02a, sea)

extracted/AWRE/assets/maps/     gitignored, written by `make setup`
  campaign/       MapHeader.mapCase 0 and 1: missions and the second maps of two-map missions
  versus/         cases 2-6
  warroom/        cases 7 and 8
  survival/       case 9 (the maps of gSurvivalMapsMode0-5)
  other/          map 0 (case 0xffff) and the Design Room slots (case 10)
```

Each YAML file is named `NNN_name.yaml`, with the map id in hex and the map's name from the game text. The three `.txt` files decide the order of everything in ov000; the order of the unit lists is not the order of the map ids.

A map file:

```yaml
# Example Island (map 0x0ff, made-up values)
header:
  script: null                       # pointer fields: a symbol from symbols.txt, or null
  name: {index: 0x123, group: 0xc000}
  playerCount: 2
  mapCase: 7
  co: [[CO_NONE, CO_NONE], [CO_ANDY, CO_NONE], ...]
  ...
tiles:
  rows: |                            # one line per row, one token per tile (legend.yaml)
    o   o   o   o   o   o   o   o
    o   o2  o3  o3  o3  o3  o4  o
    o2  o9  .   .   ^   .   o10 o4
    ...
hardTiles: ...                       # only on the 4 maps that have them
units:
- army: 1                            # player slot of the rows that follow
  rows:
  - {x: 3, y: 2, type: UNIT_TANK, flags: 0, hp: 100, ammo: 9, fuel: 70, cargo: [0, 0], aiOrder: 0, unk_0a: 0, aiTargetX: 3, aiTargetY: 2}
hardUnits: ...
```

- `tiles` and `hardTiles` hold the map as a block of tokens; the width and height come from the block. Map 0xf8 adds `legend: bmap002` (below).
- `header` holds every `MapHeader` field by its C name, except `mapData`, `hardMapData`, `units` and `hardUnits`: those point at the `tiles`, `hardTiles`, `units` and `hardUnits` sections. `pad_` fields may be left out; they are zero.
- A word in capitals is a constant from `include/` (`CO_ANDY`, `UNIT_TANK`). Any other word is a symbol, written into the C as `extern char NAME[]`.
- A unit list is written to the ROM as an army row (`PREDEPLOYED_ARMY`, slot) before each army's rows, then an end row. A list can be empty (`units: []`): the header still points at an end row.
- When a header field gets a new name in `include/`, rename the key in the YAML files too; `pack` stops on a key the struct does not have.

## Tile legend

`assets/maps/legend.yaml` has one section per tile code table. `bmap003` is for every map but one; `bmap002` is for the bottom map of Means to an End (0xf8), whose table turns many sea, shoal and river codes into the Grand Bolt. Each line gives a token and its tile code, and a comment with the terrain id and owner the table gives that code.

`unpack` makes the tokens from the terrain id: a symbol per terrain (`.` plain, `~` river, `^` mountain, `T` wood, `=` road, `o` sea, `,` shoal, `b` bridge, `|` pipe, `r` reef, `x` the other tiles of a large object, `G` and `g` the Grand Bolt's body and weak points). Owned terrain adds the owner: `C0` is a neutral city, `H1` player 1's HQ, `B2` a base of player 2, `m0`-`m3` the four Minicannon codes. The other terrain has many codes for its shapes (coasts, road bends): the code the maps use most gets the bare symbol and the rest are numbered in code order (`o`, `o1`, `o2`). The numbers say nothing about the shape yet.

Tokens can be renamed in `legend.yaml` (one word, not starting with `@`, one token per code); run `unpack` afterwards to rewrite the maps with the new names, as it keeps an existing `legend.yaml`. A map can also give a code directly as `@` and hex (`@1a4`), for a code with no token. `pack` stops on a token the map's section does not have.

## Build

1. `tools/maps.py pack` (the `MAPS` step) reads the files above and writes `build/gen/maps/`: `map_data.h` (the `MapTileData` struct, one member per tile block, and one `PredeployedUnit` array per unit list), `map_tiles.inc`, `map_units.inc` and `map_headers.inc`.
2. `src/game/ov000/map/map_tiles.c`, `map_headers.c` and `map_units.c` include those files. `config/AWRE/arm9/overlays/ov000/delinks.txt` gives them the three ranges in the table above. `configure.py` adds `-i build/gen` to the compiler flags.
3. `tools/maps.py lcf` writes the symbols that other code uses inside `Ov000_gMapHeaders` (field bases such as `data_ov000_022dbd50`) into `build/arm9.lcf` as `Ov000_gMapHeaders + offset`, right after the line that places `map_headers.o`. Above that line the linker takes the table's address as 0.

`pack` compresses each tile block itself: longest match within 0x1000 bytes, the nearest of equal matches. That gives the ROM's bytes for all 260 blocks. It reads the field lists of `MapHeader`, `PredeployedUnit`, `MapSlotFlags` and `TextRef` from `include/` and writes the initializers in declaration order, because mwccarm 2.0 has no designated initializers (`{ .x = 1 }` is "illegal initialization").

The C has the same symbols as the ROM, so that objdiff can compare the tables symbol by symbol (issue 0082):

- `Ov000_gMapTiles`: all tile blocks, one symbol. The blocks are in map id order, not size order, so they cannot be separate objects of one C file (below). The header table's relocations point into it as `to:0x022b386c add:OFFSET` in `config/AWRE/arm9/overlays/ov000/relocs.txt`.
- `Ov000_gMapUnits_0f0`, `Ov000_gMapHardUnits_0f0`: one array per unit list. `symbols.txt` gives each its size without the pad bytes (`kind:data(byte[468])`), since the C array has none; the linker's `ALIGNALL(4)` adds them.
- `Ov000_gMapHeaders`: the header table.

mwccarm 2.0 does not emit a file's data objects in the order they are defined. It puts each in its own section and heapsorts them by size, starting, for initialized data, from the reverse of the definition order; objects of equal size come out in an order that depends only on their positions. The unit lists in the ROM are in size order, so the original was one file with one array per list. `maps.py` uses the heapsort's inverse (`definition_order` in `tools/mwcc_order.py`) to find the order to define the arrays in so that the compiler emits the `units.txt` order.

dsd had taken 38 runs of bytes in the tile blocks and unit lists for pointers. Those relocations are gone from `relocs.txt`; the tables hold no pointers.

## Editing a map

Edit the YAML file under `extracted/` and run `make`. `make setup` unpacks the maps again and overwrites such edits, so keep edited maps in the mod build and point `maps.py pack --maps` at them. A change that alters the size of a tile block or unit list moves everything after it in ov000. Such a build no longer matches retail: it belongs in the mod build (issue 0023), not in a commit here.

To regenerate the map files from the ROM: `make assets` (or `python3 tools/maps.py unpack`). It reads `extract/` and the field offsets in `build/dsindex.json` (`tools/dsindex.py`) and overwrites `extracted/AWRE/assets/maps/`. It reads `legend.yaml` and writes it only when there is none. It checks the three glue files against the ROM's order and stops if they differ; `--write-glue` rewrites them.

## Python packages

`tools/maps.py` and `tools/configure.py` need PyYAML (`python3-yaml` on Ubuntu).
