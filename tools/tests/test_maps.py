#!/usr/bin/env python3
"""Check how maps.py pack reads the symbols a map file names.

    python3 tools/tests/test_maps.py

- current_name: a known symbol stays as written.
- stale_overlay, stale_main: dsd's name of a symbol renamed since the map
  file was unpacked (data_ov001_02353ba8, data_0215b9d4) becomes the symbol
  at that address now.
- no_symbol_there, wrong_module, other_word: a name with no symbol at its
  address, a module that has no symbol there, and a word of another form
  stay as written, so the link names them.
- c_scalar: the pointer field's C uses the current name and declares it.
- array_lengths: a struct field's array length written with #define names
  from any header (ARMY_SLOT_COUNT, a sum of them) reads as its number.
- enum_expressions: an enum constant written as `A | B` or `A + 1` gets its
  value (CO_CLONE_DRAKE = CO_CLONE | CO_DRAKE), and value 138 reads back as
  its name.

Builds a stand-in config/AWRE/arm9/ in a temporary directory, so no ROM or
build is needed.
"""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import maps  # noqa: E402

failures = []


def expect(what: str, got, want) -> None:
    print(f"{what}: {'ok' if got == want else 'FAIL'}{'' if got == want else f' (got {got!r}, want {want!r})'}")
    if got != want:
        failures.append(what)


with tempfile.TemporaryDirectory() as tmp:
    root = Path(tmp)
    config = root / maps.CONFIG
    (config / "overlays/ov001").mkdir(parents=True)
    (config / "symbols.txt").write_text(
        "gCampaignMissions kind:data(any) addr:0x0215ba04\n"
        "Main_gRenamed kind:data(any) addr:0x0215b9d4\n")
    (config / "overlays/ov001/symbols.txt").write_text(
        "Ov001_gMapScript_0e0 kind:data(any) addr:0x02353ba8\n"
        "data_ov001_02353be4 kind:data(any) addr:0x02353be4\n")
    maps.ROOT = root
    names = maps.SymbolNames()
    expect("current_name", names.current("Ov001_gMapScript_0e0"), "Ov001_gMapScript_0e0")
    expect("current_dsd_name", names.current("data_ov001_02353be4"), "data_ov001_02353be4")
    expect("stale_overlay", names.current("data_ov001_02353ba8"), "Ov001_gMapScript_0e0")
    expect("stale_main", names.current("data_0215b9d4"), "Main_gRenamed")
    expect("no_symbol_there", names.current("data_ov001_02353bac"), "data_ov001_02353bac")
    expect("wrong_module", names.current("data_ov002_02353ba8"), "data_ov002_02353ba8")
    expect("other_word", names.current("gGone"), "gGone")
    maps.symbol_names.cache_clear()
    externs: set[str] = set()
    expect("c_scalar", maps.c_scalar("data_ov001_02353ba8", "MapScript *", "test", externs),
           "(void *)Ov001_gMapScript_0e0")
    expect("c_scalar_extern", externs, {"Ov001_gMapScript_0e0"})

with tempfile.TemporaryDirectory() as tmp:
    root = Path(tmp)
    (root / maps.INCLUDE / "game").mkdir(parents=True)
    (root / maps.INCLUDE / "game/co.h").write_text(
        "enum CoId {\n  CO_NONE,\n  CO_DRAKE = 10,\n  CO_COUNT,\n  CO_PICKS = CO_COUNT, /* note */\n"
        "  CO_CLONE = 0x80,\n  CO_CLONE_DRAKE = CO_CLONE | CO_DRAKE,\n  CO_NEXT = CO_DRAKE + 2\n};\n")
    maps.ROOT = root
    values = maps.enum_values("game/co.h", "CoId")
    expect("enum_expressions", (values["CO_PICKS"], values["CO_CLONE_DRAKE"], values["CO_NEXT"]), (11, 138, 12))
    expect("enum_names", maps.enum_names("game/co.h", "CoId")[138], "CO_CLONE_DRAKE")

with tempfile.TemporaryDirectory() as tmp:
    root = Path(tmp)
    (root / maps.INCLUDE / "game").mkdir(parents=True)
    (root / maps.INCLUDE / "game/player.h").write_text(
        "#define SLOTS 5 /* note */\n#define LAST (SLOTS - 1)\n#define COS 2\n")
    (root / maps.INCLUDE / "game/map_header.h").write_text(
        '#include "game/player.h"\ntypedef struct MapHeader {\n  u8 co[SLOTS]\n       [COS]; /* x */\n'
        "  u8 team[LAST + 0x2];\n  u8 pad[4];\n} MapHeader;\n")
    maps.ROOT = root
    maps.c_defines.cache_clear()
    maps.c_fields.cache_clear()
    expect("array_lengths", [f[2] for f in maps.c_fields("MapHeader")], [[5, 2], [6], [4]])

if failures:
    print(f"test_maps: {len(failures)} failed: {', '.join(failures)}")
    sys.exit(1)
print("test_maps: all passed")
