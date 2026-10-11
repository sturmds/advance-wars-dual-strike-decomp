#!/usr/bin/env python3
"""Check tools/pragmas.py without a ROM.

    python3 tools/tests/test_pragmas.py

1. mwccarm 2.0-72 accepts every pragma the sweep uses.
2. The `func` placement wraps only the named function, and skips its
   prototype.
3. A fixture compiled with `#pragma optimization_level 1` around one function
   is the reference; the sweep over the same C without the pragma finds a
   setting that matches it, and the build flags alone do not.

Needs the compiler from `make setup`; skipped without it.
"""

import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import configure  # noqa: E402
import pragmas  # noqa: E402

ROOT = pragmas.ROOT
failures = []


def check(what: str, ok: bool, detail=""):
    print(f"{what}: {'ok' if ok else 'FAIL'}{f' ({detail})' if detail and not ok else ''}")
    if not ok:
        failures.append(what)


FIXTURE = """typedef struct Pad { unsigned short held; unsigned short repeat; } Pad;
extern Pad *gPad;
int Fixture_Other(int a);
int Fixture_Step(int x);

int Fixture_Other(int a) { return gPad->repeat + a; }

int Fixture_Step(int x)
{
    if (gPad->repeat & 1)
        x--;
    if (gPad->repeat & 2)
        x++;
    return x;
}
"""


def main() -> int:
    if not pragmas.CC.exists():
        print("test_pragmas: no compiler, run make setup (skipped)")
        return 0
    check("mwccarm accepts the swept pragmas",
          subprocess.run([sys.executable, str(ROOT / "tools/pragmas.py"), "--accepted"],
                         capture_output=True, text=True).returncode == 0)

    v = pragmas.variant(FIXTURE, "Fixture_Step", (("optimization_level", "1"),), "func")
    step = v.index("int Fixture_Step(int x)\n{")
    check("func placement wraps the definition, not the prototype",
          v[:step].endswith("#pragma push\n#pragma optimization_level 1\n") and "#pragma pop" in v[step:]
          and v.index("#pragma push") > v.index("int Fixture_Other(int a) {"))

    with tempfile.TemporaryDirectory() as d:
        d = Path(d)
        ref_c, ref_o, plain = d / "ref.c", d / "ref.o", d / "plain.c"
        ref_c.write_text(v)
        plain.write_text(FIXTURE)
        r = subprocess.run([str(pragmas.CC), "-c", *configure.CFLAGS.split(), "-o", str(ref_o), str(ref_c)],
                           cwd=ROOT, capture_output=True, text=True)
        check("fixture compiles", r.returncode == 0, r.stdout + r.stderr)
        if r.returncode:
            return 1
        S = pragmas.Sweep(plain, "Fixture_Step", pragmas.ObjectTarget(ref_o, "Fixture_Step"), jobs=2)
        try:
            base = S.sweep()
            best = S.ranked()[0]
        finally:
            S.close()
        check("build flags alone differ from the reference", not base["ok"] and base["differ"] > 0)
        check("the sweep finds a matching setting", best[1]["ok"], pragmas.setting_text(best[0][0]))
    print("test_pragmas:", "FAILED " + ", ".join(failures) if failures else "all ok")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
