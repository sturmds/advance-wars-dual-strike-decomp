#!/usr/bin/env python3
"""Check tools/protocheck.py on a made-up tree, without a ROM.

    python3 tools/tests/test_protocheck.py

The tree has a header, three definitions and a partial that declares them
the way old guesses do: too few arguments, an int where the definition
takes u8, a struct pointer typed void *, and calls that match their wrong
declaration. Each kind must be reported once, with nothing else.
"""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import protocheck  # noqa: E402

failures = []


def check(what: str, ok: bool, detail=""):
    print(f"{what}: {'ok' if ok else 'FAIL'}{f' ({detail})' if detail and not ok else ''}")
    if not ok:
        failures.append(what)


FILES = {
    "include/types.h": """
typedef unsigned int u32;
typedef unsigned char u8;
typedef int BOOL;
typedef enum Side { SIDE_LEFT, SIDE_RIGHT } Side;
typedef struct Task { int x; } Task;
""",
    "include/game/sprite.h": """
#include "types.h"
/* A prototype with no definition in src/. */
void Sprite_Free(Task *task, u32 slot);
""",
    "src/game/cell.c": """
#include "types.h"
void Cell_Start(int side, int cell, int param)
{
    (void)side; (void)cell; (void)param;
}
""",
    "src/game/blend.c": """
#include "types.h"
BOOL Blend_Set(Task *task, u8 level, Side side) { return task->x + level + side; }
static int Helper(int a, int b) { return a + b; }
""",
    "src/game/printf.c": """
int Log_Print(const char *fmt, ...) { return 0; }
""",
    "src/arm7/os.c": """
void Os_Init(int a, int b) { }
""",
    "src/game/partial.c": """
#include "types.h"
#include "game/sprite.h"

void Cell_Start(int side, int unk_3);          /* count: definition has 3 */
int Blend_Set(void *task, int level, int side); /* width x2, pointer only */
int Helper(int a);                              /* static elsewhere: no reference */
void Sprite_Free(Task *task);                   /* count against the header */
int Log_Print(const char *fmt, ...);
void Os_Init(int a);                            /* ARM7 definition: not seen */
struct Point { int x, y; };
static const struct Point gOrigin = { 0, 0 };

/* A comment that mentions Cell_Start(1) must not count as a call. */
void Partial_Run(Task *task)
{
    const char *s = "Cell_Start(1, 2)";
    Cell_Start(0, 1);
    Blend_Set(task, 2, 1);
    Log_Print(s, 1, 2, 3);
    Sprite_Free(task);
    if (task->x) {
        Cell_Start(task->x, (task->x + 1) * 2, 3);
    }
}
""",
}


def main() -> int:
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        for rel, text in FILES.items():
            p = root / rel
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text(text)
        index = protocheck.load(root)
        rows = protocheck.report(index, ["src/game/partial.c"], everything=False)
        with_ptr = protocheck.report(index, ["src/game/partial.c"], everything=False, pointers=True)

    got = sorted((where.split(":")[1], kind, msg.split()[0]) for where, kind, msg, _ in rows)
    want = sorted([
        ("5", "count", "Cell_Start"),
        ("6", "width", "Blend_Set"),       # returns int, definition BOOL (int): no row
        ("6", "width", "Blend_Set"),
        ("8", "count", "Sprite_Free"),
        ("18", "call", "Cell_Start"),
        ("21", "call", "Sprite_Free"),
    ])
    # Blend_Set: u8 against int is a width row; Side (an enum) against int is another.
    check("rows", got == want, f"got {got}")
    check("enum differs from int", any("argument 3" in m for _, k, m, _ in rows if k == "width"))
    check("pointer-only rows hidden", not any(k == "pointer" for _, k, _, _ in rows))
    check("pointer-only rows with --pointers", any(k == "pointer" and "argument 1" in m
                                                  for _, k, m, _ in with_ptr))
    check("string and comment calls skipped", not any(w.endswith(":14") or w.endswith(":17")
                                                      for w, _, _, _ in rows))
    check("variadic call accepted", not any("Log_Print" in m for _, _, m, _ in rows))
    check("static and ARM7 definitions ignored", not any("Helper" in m or "Os_Init" in m
                                                         for _, _, m, _ in rows))

    canon = protocheck.canon
    tds = index.header_tds
    check("u32 is unsigned int", canon("u32", tds) == canon("unsigned int", {}))
    check("struct tag and typedef agree", canon("struct Task *", tds) == canon("Task *", tds))
    check("const ignored", canon("const u8 *", tds) == canon("u8 *", tds))
    check("volatile kept", canon("volatile u32 *", tds) != canon("u32 *", tds))

    if failures:
        print(f"test_protocheck: {len(failures)} failed", file=sys.stderr)
        return 1
    print("test_protocheck: ok")
    return 0


if __name__ == "__main__":
    sys.exit(main())
