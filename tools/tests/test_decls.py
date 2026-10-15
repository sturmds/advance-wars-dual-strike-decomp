#!/usr/bin/env python3
"""Check tools/decls.py on a made-up tree, without a ROM or compiler.

    python3 tools/tests/test_decls.py

check must count a C file's copy of a header's declaration and a name two
headers declare. dedupe must remove a copy with the header's type and
include the header, keep a copy with another type, and not add a header to a
file that keeps another type for one of the header's names. declare must
move a declaration to its new header as it is, and give the new include to
a file that had the name through the old header. macro must define one name
in the header for a body that two files share, rename the uses, and leave a
file whose macro of that name means something else.
"""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import decls  # noqa: E402

failures = []


def check(what: str, ok: bool, detail=""):
    print(f"{what}: {'ok' if ok else 'FAIL'}{f' ({detail})' if detail and not ok else ''}")
    if not ok:
        failures.append(what)


FILES = {
    "include/types.h": "typedef int BOOL;\ntypedef short s16;\n",
    "include/game/state.h": """\
#ifndef GAME_STATE_H
#define GAME_STATE_H
typedef struct State { int x; } State;
extern State* gState;
#endif
""",
    "include/game/unit.h": """\
#ifndef GAME_UNIT_H
#define GAME_UNIT_H
void Unit_Move(int x, int y);
BOOL Unit_IsReady(int slot);
#endif
""",
    "include/game/old.h": """\
#ifndef GAME_OLD_H
#define GAME_OLD_H
void Old_Thing(void);
#endif
""",
    "src/same.c": """\
#include "types.h"

extern State* gState;
void Unit_Move(int x, int y);

void Same(void) { Unit_Move(gState->x, 0); }
""",
    "src/narrow.c": """\
#include "types.h"

void Unit_Move(int x, int y);
BOOL Unit_IsReady(s16 slot);

void Narrow(void) { if (Unit_IsReady(1)) Unit_Move(1, 2); }
""",
    "src/user_old.c": """\
#include "types.h"
#include "game/old.h"

void UserOld(void) { Old_Thing(); }
""",
    "src/macro_a.c": """\
#include "types.h"
#include "game/state.h"

#define STATE (gState)

int MacroA(void) { return STATE->x; }
""",
    "src/macro_b.c": """\
#include "types.h"
#include "game/state.h"

#define CUR (gState)

int MacroB(void) { return CUR->x; }
""",
    "src/macro_other.c": """\
#include "types.h"

#define STATE 3

int MacroOther(void) { return STATE; }
""",
}


def tree(root: Path):
    for rel, text in FILES.items():
        (root / rel).parent.mkdir(parents=True, exist_ok=True)
        (root / rel).write_text(text)
    (root / "config/AWRE").mkdir(parents=True)
    decls.ROOT, decls.INCLUDE = root, root / "include"
    decls.EXCEPTIONS = root / "config/AWRE/local_decls.txt"


def main() -> int:
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        tree(root)
        errors = decls.check_errors()
        check("check counts the copies", any("src/same.c: gState" in e for e in errors)
              and any("src/same.c: Unit_Move" in e for e in errors), errors)

        differ = decls.dedupe(decls.Tree(), decls.c_files(None))
        same = (root / "src/same.c").read_text()
        check("dedupe removes copies with the header's type",
              "extern State* gState;" not in same and "void Unit_Move" not in same, same)
        check("dedupe includes the headers", '#include "game/state.h"' in same
              and '#include "game/unit.h"' in same, same)
        narrow = (root / "src/narrow.c").read_text()
        check("dedupe keeps a copy with another type", "BOOL Unit_IsReady(s16 slot);" in narrow
              and any("Unit_IsReady" in d for d in differ), narrow)
        check("dedupe does not add a header that would clash",
              '#include "game/unit.h"' not in narrow and "void Unit_Move(int x, int y);" in narrow,
              narrow)

        rc = decls.declare("game/new.h", ["Old_Thing"])
        new = (root / "include/game/new.h").read_text()
        old = (root / "include/game/old.h").read_text()
        user = (root / "src/user_old.c").read_text()
        check("declare moves the declaration as it is", rc == 0 and "void Old_Thing(void);" in new
              and "Old_Thing" not in old, new + old)
        check("declare gives the user the new header", '#include "game/new.h"' in user, user)
        check("declare leaves the old header's includes", '#include' not in old, old)

        class Args:
            header, new, old, body = "game/state.h", "CUR_STATE", ["STATE", "CUR"], None
            comment = "The current state."
        decls.cmd_macro(Args)
        a = (root / "src/macro_a.c").read_text()
        b = (root / "src/macro_b.c").read_text()
        other = (root / "src/macro_other.c").read_text()
        state = (root / "include/game/state.h").read_text()
        check("macro defines the name in the header", "#define CUR_STATE (gState)" in state, state)
        check("macro renames the uses", "CUR_STATE->x" in a and "CUR_STATE->x" in b
              and "#define STATE" not in a and "#define CUR" not in b, a + b)
        check("macro leaves another meaning alone", other == FILES["src/macro_other.c"], other)

    print("test_decls: " + ("all passed" if not failures else f"{len(failures)} failed"))
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
