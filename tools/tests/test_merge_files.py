#!/usr/bin/env python3
"""Check tools/merge_files.py on a made-up tree, without a ROM or compiler.

    python3 tools/tests/test_merge_files.py

Three one-function files in a row are merged. The merged file must hold the
functions last to first, each declaration once, the variable definitions in
the order mwcc_order.py gives for their addresses, and no prototype of a
merged function; the header must hold the prototypes in address order; a
caller must lose its prototype and include the header after its top
includes, not after one inside an initializer; delinks.txt must hold one
entry. A declaration of a merged function in another header moves to the
new header as it is. A gap between the files or two values for one macro
must stop the merge before it writes.
"""

import os
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import merge_files  # noqa: E402

failures = []


def check(what: str, ok: bool, detail=""):
    print(f"{what}: {'ok' if ok else 'FAIL'}{f' ({detail})' if detail and not ok else ''}")
    if not ok:
        failures.append(what)


DELINKS = """\
    .text       start:0x02000000 end:0x02100000 kind:code align:32

src/game/main/ai/a.c:
    complete
    .text       start:0x02000000 end:0x02000010

src/game/main/ai/b.c:
    complete
    .text       start:0x02000010 end:0x02000020
    .bss        start:0x02200000 end:0x02200008

src/game/main/ai/c.c:
    complete
    .text       start:0x02000020 end:0x02000030
    .bss        start:0x02200008 end:0x0220000c

src/game/main/ai/far.c:
    complete
    .text       start:0x02000040 end:0x02000050
"""

SYMBOLS = """\
data_b0 kind:bss(size=0x4) addr:0x02200000
data_b1 kind:bss(size=0x4) addr:0x02200004
data_c0 kind:bss(size=0x4) addr:0x02200008
"""

FILES = {
    "include/types.h": "typedef int BOOL;\n",
    "include/game/unit.h": "typedef struct Unit { int x; } Unit;\n",
    "src/game/main/ai/a.c": """\
#include "types.h"

#define STATE (gState)

extern int gState;
void Ai_B(Unit* unit);

/* First function. */
void Ai_A(void) { Ai_B(0); }
""",
    "src/game/main/ai/b.c": """\
#include "types.h"
#include "game/unit.h"

#define STATE (gState)

extern int gState;
int data_b1;
int data_b0;

void Ai_B(Unit* unit) { data_b0 = STATE; }
""",
    "src/game/main/ai/c.c": """\
#include "types.h"

extern int gState;
int data_c0;

BOOL Ai_C(int x, int y) { return x + y + data_c0; }
""",
    "src/game/main/ai/far.c": "void Ai_Far(void) {}\n",
    "src/game/main/caller.c": """\
#include "types.h"

int gTable[] = {
#include "data/table.inc.c"
};

void Ai_A(void);
BOOL Ai_C(int, int);
void Other(void);

void Caller(void) { Ai_A(); Ai_C(1, 2); }
""",
}


def tree(root: Path, extra=None):
    files = dict(FILES, **(extra or {}))
    for rel, text in files.items():
        (root / rel).parent.mkdir(parents=True, exist_ok=True)
        (root / rel).write_text(text)
    cfg = root / "config/AWRE/arm9"
    cfg.mkdir(parents=True)
    (cfg / "delinks.txt").write_text(DELINKS)
    (cfg / "symbols.txt").write_text(SYMBOLS)
    env = {k: v for k, v in os.environ.items() if not k.startswith("GIT_")}
    for cmd in (["init", "-q"], ["add", "-A"]):
        subprocess.run(["git", *cmd], cwd=root, check=True, env=env)


def run(root: Path, *files: str) -> int:
    merge_files.ROOT = root
    merge_files.CONFIG = root / "config/AWRE"
    merge_files._defs = None
    sys.argv = ["merge_files.py", "src/game/main/ai/abc.c", *files]
    return merge_files.main()


def main() -> int:
    abc = ["src/game/main/ai/a.c", "src/game/main/ai/b.c", "src/game/main/ai/c.c"]
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        tree(root)
        rc = run(root, *abc)
        check("merge exits 0", rc == 0, rc)
        out = (root / "src/game/main/ai/abc.c").read_text()
        pos = [out.find("void Ai_A(void) {"), out.find("void Ai_B(Unit* unit) {"),
               out.find("BOOL Ai_C(int x, int y) {")]
        check("functions last to first", -1 not in pos and pos[2] < pos[1] < pos[0], pos)
        check("comment kept with its function", "/* First function. */\nvoid Ai_A" in out)
        check("macro once", out.count("#define STATE") == 1)
        check("extern once", out.count("extern int gState;") == 1)
        check("no prototype of a merged function", "void Ai_B(Unit* unit);" not in out)
        check("header included", '#include "game/ai/abc.h"' in out)
        # Uninitialized, all size 4, at b0, b1, c0: mwcc emits them in that
        # order when they are defined as c0, b0, b1 (mwcc_order.py).
        order = [out.find(f"int {n};") for n in ("data_c0", "data_b0", "data_b1")]
        check("variables in mwcc order", -1 not in order and order == sorted(order), order)
        hdr = (root / "include/game/ai/abc.h").read_text()
        check("header prototypes in address order",
              hdr.find("void Ai_A(void);") < hdr.find("void Ai_B(Unit* unit);")
              < hdr.find("BOOL Ai_C(int x, int y);"), hdr)
        check("header includes the type's header", '#include "game/unit.h"' in hdr, hdr)
        caller = (root / "src/game/main/caller.c").read_text()
        check("caller loses its prototypes", "void Ai_A(void);" not in caller
              and "BOOL Ai_C(int, int);" not in caller and "void Other(void);" in caller, caller)
        check("caller includes the header at the top",
              caller.startswith('#include "types.h"\n#include "game/ai/abc.h"\n'), caller)
        delinks = (root / "config/AWRE/arm9/delinks.txt").read_text()
        check("one delinks entry", "src/game/main/ai/abc.c:\n    complete\n"
              "    .text       start:0x02000000 end:0x02000030\n"
              "    .bss        start:0x02200000 end:0x0220000c\n" in delinks
              and "ai/a.c:" not in delinks and "ai/far.c:" in delinks, delinks)
        check("old files removed", not any((root / f).exists() for f in abc))

    stops = {
        "a gap stops it": ({}, abc[:2] + ["src/game/main/ai/far.c"]),
        "two macro values stop it": (
            {"src/game/main/ai/c.c": '#define STATE (0)\nint Ai_C(void) { return STATE; }\n'}, abc),
    }
    for what, (extra, files) in stops.items():
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            tree(root, extra)
            try:
                rc = run(root, *files)
            except SystemExit as e:
                rc = e.code
            check(what, rc not in (0, None) and not (root / "src/game/main/ai/abc.c").exists(), rc)

    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        tree(root, {"include/game/old.h": '#include "game/unit.h"\nvoid Ai_B(Unit* u);\n'})
        rc = run(root, *abc)
        old = (root / "include/game/old.h").read_text()
        hdr = (root / "include/game/ai/abc.h").read_text()
        check("another header's declaration moves as it is",
              rc == 0 and "Ai_B" not in old and "void Ai_B(Unit* u);" in hdr
              and "void Ai_B(Unit* unit);" not in hdr, hdr)

    print("test_merge_files: " + ("all passed" if not failures else f"{len(failures)} failed"))
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
