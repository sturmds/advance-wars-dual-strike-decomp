#!/usr/bin/env python3
"""Checks tools/bss.py on made-up symbol lists (docs/bss-tool.md).

    python3 tools/tests/test_bss.py

No ROM needed. Covers variables, aliases, filler and declared sizes
(build_items), the filler cuts that keep runs few (fill), the run split,
dsd's link-order check and the linker script lines. With the compiler from
`make setup`, also compiles a file of three runs written by body_lines and
checks its sections. With build/fnindex.json and build/dsindex.json, runs
`bss.py plan --show` twice on each overlay and compares the output.
"""

import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import bss  # noqa: E402
import pragmas  # noqa: E402

ROOT = bss.ROOT
failures = []


def check(what: str, ok: bool, detail="") -> None:
    print(f"{what}: {'ok' if ok else 'FAIL'}{f' ({detail})' if detail and not ok else ''}")
    if not ok:
        failures.append(what)


def layout(variables: list) -> list[tuple[int, int, str]]:
    return [(v.addr, v.size, v.name) for v in variables]


def test_items() -> None:
    syms = [("a", 0x100, 6, None),     # declared 6 bytes: 8 with the pad, then 8 bytes of filler
            ("a_inner", 0x104, None, None),  # inside a's declared size: alias
            ("b", 0x110, None, None),  # no size known: runs to c
            ("b_odd", 0x113, None, None),    # unaligned: alias
            ("c", 0x120, 4, None),     # declared 4, the rest of the gap is filler
            ("d", 0x130, None, 0x18)]  # u8 d[] of 0x18-byte elements: one fits in 0x20 bytes
    items, aliases = bss.build_items(syms, 0x100, 0x150, "data_")
    shape = [(type(x).__name__, x.addr, x.size) for x in items]
    check("build_items: variables and filler", shape == [
        ("Var", 0x100, 8), ("Filler", 0x108, 8), ("Var", 0x110, 0x10), ("Var", 0x120, 4),
        ("Filler", 0x124, 0xc), ("Var", 0x130, 0x18), ("Filler", 0x148, 8)], shape)
    check("build_items: sort size of a declared variable", items[0].sort == 6)
    check("build_items: aliases", aliases == [("a_inner", 0x104), ("b_odd", 0x113)], aliases)
    variables = bss.fill(items, "data_", split=False)
    bss.attach_aliases(variables, aliases)
    owners = {n: v.name for v in variables for n, _ in v.aliases}
    check("attach_aliases", owners == {"a_inner": "a", "b_odd": "b"}, owners)
    check("placeholder names", [v.name for v in variables if v.placeholder]
          == ["data_00000108", "data_00000124", "data_00000148"])


def test_filler_cuts() -> None:
    # 4, then 0x14 bytes of filler, then 8: one piece would go down at 8.
    items = [bss.Var(0, 4, 4, "a"), bss.Filler(4, 0x14, "b"), bss.Var(0x18, 8, 8, "b")]
    whole = bss.fill(items, "p_", split=False)
    cut = bss.fill(items, "p_", split=True)
    check("fill: one piece gives two runs", len(bss.split_runs(whole)) == 2)
    check("fill: cut pieces keep one run", len(bss.split_runs(cut)) == 1, [v.size for v in cut])
    check("fill: fewest pieces", [v.size for v in cut] == [4, 4, 8, 8, 8], [v.size for v in cut])
    # Filler that fits as one piece stays one piece.
    items = [bss.Var(0, 4, 4, "a"), bss.Filler(4, 8, "b"), bss.Var(0xc, 0x10, 0x10, "b")]
    check("fill: no cut when one piece keeps the order",
          [v.size for v in bss.fill(items, "p_")] == [4, 8, 0x10])
    # Less than 32 bytes before a 32-aligned variable: alignment, no placeholder.
    items = [bss.Var(0x100, 0x10, 0x10, "a"), bss.Filler(0x110, 0x10, "b"), bss.Var(0x120, 4, 4, "b")]
    got = bss.fill(items, "p_")
    check("fill: alignment instead of filler", layout(got) == [(0x100, 0x10, "a"), (0x120, 4, "b")]
          and got[1].align32, layout(got))
    check("fill: does not change its input", not items[2].align32)


def test_split_filler() -> None:
    check("split_filler: start of a gap", bss.split_filler(0x10, 0, 4, False) == (0, [4, 4, 4, 4]))
    check("split_filler: end of a gap", bss.split_filler(0x10, 8, float("inf"), False) == (0, [0x10]))
    check("split_filler: forced descent", bss.split_filler(8, 0x20, 4, False)[0] == 1)


def test_runs() -> None:
    sizes = [4, 4, 8, 4, 0x10, 0xc, 0xc]
    variables = [bss.Var(i * 0x10, s, s, f"v{i}") for i, s in enumerate(sizes)]
    runs = bss.split_runs(variables)
    check("split_runs", [[v.sort for v in r] for r in runs] == [[4, 4, 8], [4, 0x10], [0xc, 0xc]])


def test_link_order() -> None:
    a = bss.Entry("a.c", True, {".text": (0x100, 0x200)})
    b = bss.Entry("b.c", True, {".text": (0x200, 0x300)})
    check("link_order_ok: one order", bss.link_order_ok([
        bss.Entry(a.name, True, {**a.sections, ".bss": (0x1000, 0x1010)}),
        bss.Entry(b.name, True, {**b.sections, ".bss": (0x1010, 0x1020)})]))
    check("link_order_ok: a cycle", not bss.link_order_ok([
        bss.Entry(a.name, True, {**a.sections, ".bss": (0x1010, 0x1020)}),
        bss.Entry(b.name, True, {**b.sections, ".bss": (0x1000, 0x1010)})]))


def test_lcf() -> None:
    entry = bss.Entry("src/x/bss_x.c", True, {".bss": (0x100, 0x130)})
    symbols = [bss.Symbol("v0", "bss", 0x100, 8), bss.Symbol("v1", "bss", 0x108, 4),
               bss.Symbol("v1_b", "bss", 0x10a, 0), bss.Symbol("v2", "bss", 0x10c, 0x24)]
    anchor, lines = bss.lcf_object_lines(entry, symbols)
    check("lcf_object_lines", (anchor, lines) == ("bss_x.o(.bss)", ["bss_x.o(.bss.run1)", "v1_b = v1 + 0x2;"]),
          lines)
    text = "    .ov : {\n        bss_x.o(.bss)\n    }\n"
    once = bss.lcf_insert_after(text, anchor, lines, "test")
    check("lcf_insert_after: twice changes nothing", bss.lcf_insert_after(once, anchor, lines, "test") == once)


class NoDecls:
    def of(self, name: str) -> list:
        return []


class FakeIndex:
    decls = NoDecls()


def test_compile() -> None:
    if not pragmas.CC.exists():
        print("test_bss: no compiler, run make setup (compile test skipped)")
        return
    sizes = [[4, 4, 8], [4, 0x10], [8, 8]]
    runs, addr = [], 0
    for run in sizes:
        runs.append([])
        for s in run:
            runs[-1].append(bss.Var(addr, s, s, f"v_{addr:03x}"))
            addr += s
    g = bss.Group(runs, "src/x", path="src/x/bss_test.c")
    lines, headers = bss.body_lines(FakeIndex(), g, set())
    elf = bss.compile_probe(g.path, bss.new_file_text(lines, headers, True))
    check("three runs compile", elf is not None, bss.compile_errors)
    if elf is not None:
        errors = bss.order_errors(elf, g)
        check("three runs come out in order", not errors, errors)


def test_twice() -> None:
    if not (bss.FNINDEX.exists() and bss.DSINDEX.exists() and pragmas.CC.exists()):
        print("test_bss: no build/fnindex.json, build/dsindex.json or compiler (plan test skipped)")
        return
    for module in bss.module_modules():
        if not module.startswith("ov"):
            continue
        cmd = [sys.executable, "tools/bss.py", "plan", module, "--show"]
        first = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
        second = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
        check(f"plan {module} --show runs", first.returncode == 0, first.stderr.strip())
        check(f"plan {module} --show gives the same output twice", first.stdout == second.stdout)


def main() -> int:
    test_items()
    test_filler_cuts()
    test_split_filler()
    test_runs()
    test_link_order()
    test_lcf()
    test_compile()
    test_twice()
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
