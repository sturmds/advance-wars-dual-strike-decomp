#!/usr/bin/env python3
"""Checks tools/bss.py on made-up symbol lists (docs/bss-tool.md).

    python3 tools/tests/test_bss.py

No ROM needed. Covers variables, aliases, filler and declared sizes
(build_items), aliases past a variable's declared size, the filler cuts
that keep runs few (fill), the run split, the slicing rule (best_host,
slice_run, the filler that goes to the file before a 32-aligned one, the
run counts of a --range), dsd's link-order check, the parts of an
ARM7-like module, the linker script lines for .bss and .sbss, the
generated definitions (`bss.py gen` on a made-up config, under two hash
seeds) and the definitions `bss.py move` takes out of a file. best_host
runs under two hash seeds and must pick the file first in fnindex order
both times. With the compiler from `make setup`, also compiles files
written by body_lines and checks their sections: three .bss runs with the
ARM9 flags, and two .sbss runs and three .bss runs with the ARM7 flags; a
pointer and an array aligned to 32 (in .bss and .bss.run1); and the
generated definitions. With build/fnindex.json and build/dsindex.json,
runs `bss.py plan --show` under PYTHONHASHSEED 1 and 2 on every module
and compares the output.
"""

import os
import shutil
import subprocess
import sys
import tempfile
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
    items, aliases, past = bss.build_items(syms, 0x100, 0x150, "data_")
    check("build_items: no alias past a declared size", past == [], past)
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
    anchor, lines = bss.lcf_object_lines(entry, ".bss", symbols)
    check("lcf_object_lines", (anchor, lines) == ("bss_x.o(.bss)", ["bss_x.o(.bss.run1)", "v1_b = v1 + 0x2;"]),
          lines)
    text = "    .ov : {\n        bss_x.o(.bss)\n    }\n"
    once = bss.lcf_insert_after(text, anchor, lines, "test")
    check("lcf_insert_after: twice changes nothing", bss.lcf_insert_after(once, anchor, lines, "test") == once)
    entry = bss.Entry("src/arm7/x/os_bss.c", True, {".sbss": (0x200, 0x218)})
    symbols = [bss.Symbol("w", "bss", 0x200, 2), bss.Symbol("t", "bss", 0x204, 0xc),
               bss.Symbol("t_arg", "bss", 0x20c, 0), bss.Symbol("p", "bss", 0x210, 8)]
    anchor, lines = bss.lcf_object_lines(entry, ".sbss", symbols)
    check("lcf_object_lines: .sbss", (anchor, lines) == ("os_bss.o(.sbss)", ["os_bss.o(.sbss.run1)", "t_arg = t + 0x8;"]),
          (anchor, lines))


def test_sections() -> None:
    check("run_section", [bss.run_section(".bss", 0), bss.run_section(".bss", 2), bss.run_section(".sbss", 0),
                          bss.run_section(".sbss", 1)] == [".bss", ".bss.run2", ".sbss", ".sbss.run1"])
    check("run_pragma", [bss.run_pragma(".bss", 0), bss.run_pragma(".bss", 2), bss.run_pragma(".sbss", 0),
                         bss.run_pragma(".sbss", 1)] == [None, "BSS_RUN2", "SBSS", "SBSS_RUN1"])
    header = (ROOT / "include" / bss.RUN_HEADER).read_text()
    for section, limit in bss.MAX_RUNS.items():
        names = [bss.run_pragma(section, k) for k in range(limit)]
        missing = [n for n in names if n and f"#pragma define_section {n} " not in header]
        check(f"{bss.RUN_HEADER} defines every {section} run bss.py may write", not missing, missing)


def test_segments() -> None:
    # Three parts in the order ARM7's delinks.txt lists them: each starts with its .text.
    module = bss.Module.__new__(bss.Module)
    module.header = [(".text", 0x2380000, 0x2380158), (".bss", 0x2380180, 0x2380180),
                     (".text", 0x27c0000, 0x27d6570), (".bss", 0x27d6c60, 0x27d8518),
                     (".text", 0x37f8000, 0x380773c), (".sbss", 0x3807888, 0x38078a0),
                     (".bss", 0x38078a0, 0x380b004)]
    got = [module.segment(a) for a in (0x2380180, 0x27d0000, 0x27d8000, 0x37fe910, 0x3807888, 0x380b000, 0x2000000)]
    check("Module.segment", got == [0, 1, 1, 2, 2, 2, None], got)


def test_past_aliases() -> None:
    # A 2-byte variable with an alias at +2 (in its pad) and one with an alias at +1.
    syms = [("a", 0x100, 2, None), ("a_2", 0x102, None, None),
            ("b", 0x104, 2, None), ("b_1", 0x105, None, None)]
    _, aliases, past = bss.build_items(syms, 0x100, 0x108, "data_")
    check("build_items: alias at the declared size", [(p.alias, p.var, p.declared, p.offset) for p in past]
          == [("a_2", "a", 2, 2)], past)
    check("build_items: alias inside the declared size is no problem", ("b_1", 0x105) in aliases)


class FakeUsers:
    """Users with made-up files: rank gives the fnindex order."""

    def __init__(self, files: dict[str, set[str]], rank: dict[str, int]) -> None:
        self.files = files
        self.rank = rank
        self.local_files = set(rank)

    def local_of(self, v) -> list[str]:
        names = [v.name] + [a for a, _ in v.aliases]
        return sorted(set().union(*(self.files.get(n, set()) for n in names)), key=self.rank.__getitem__)


BEST_HOST_TIE = """
import sys
sys.path.insert(0, sys.argv[1])
import bss
from tests.test_bss import FakeUsers
users = FakeUsers({"v": {"src/a.c", "src/b.c", "src/c.c"}}, {"src/c.c": 0, "src/b.c": 1, "src/a.c": 2})
print(bss.best_host(users, bss.Var(0, 4, 4, "v"), None, lambda f: f != "src/c.c"))
"""


def test_best_host() -> None:
    users = FakeUsers({"v": {"src/a.c", "src/b.c"}}, {"src/b.c": 0, "src/a.c": 1})
    v = bss.Var(0, 4, 4, "v")
    check("best_host: first in fnindex order", bss.best_host(users, v, None, lambda f: True) == "src/b.c")
    check("best_host: the choice of the variable before", bss.best_host(users, v, "src/a.c", lambda f: True)
          == "src/a.c")
    check("best_host: no allowed file", bss.best_host(users, v, None, lambda f: False) is None)
    tools = str(ROOT / "tools")
    got = []
    for seed in ("1", "2"):
        r = subprocess.run([sys.executable, "-c", BEST_HOST_TIE, tools], capture_output=True, text=True,
                           env={**os.environ, "PYTHONHASHSEED": seed})
        got.append(r.stdout.strip() or r.stderr.strip())
    check("best_host: a tie goes the same way under two hash seeds", got == ["src/b.c", "src/b.c"], got)


def test_slice_run() -> None:
    run = [bss.Var(0x0, 4, 4, "u"), bss.Var(0x4, 4, 4, "v0"), bss.Var(0x8, 4, 4, "data_00000008", placeholder=True),
           bss.Var(0xc, 8, 8, "v2"), bss.Var(0x14, 8, 8, "unused"), bss.Var(0x1c, 8, 8, "v4")]
    users = FakeUsers({"v0": {"src/a.c"}, "v2": {"src/b.c"}, "v4": {"src/a.c", "src/b.c"}},
                      {"src/a.c": 0, "src/b.c": 1})
    got = [(h, [v.name for v in vs]) for h, vs in bss.slice_run(None, users, run, lambda f, v: True)]
    check("slice_run: cut where the host changes", got == [
        ("src/a.c", ["u", "v0", "data_00000008"]), ("src/b.c", ["v2", "unused", "v4"])], got)
    got = [(h, len(vs)) for h, vs in bss.slice_run(None, users, run, lambda f, v: False)]
    check("slice_run: no allowed host gives one slice", got == [(None, 6)], got)


def fake_module(gaps: list) -> "bss.Module":
    module = bss.Module.__new__(bss.Module)
    module.name = "ov099"
    module.prefix = "data_ov099_"
    module.entries = []
    module.index = FakeIndex()
    module.gaps = gaps
    module.header = []
    return module


def test_tails() -> None:
    # 0x8 bytes no symbol names before a 32-aligned variable that starts a slice.
    a = bss.Var(0x100, 0x18, 0x18, "a")
    b = bss.Var(0x120, 4, 4, "b", align32=True)
    module = fake_module([bss.Gap(".bss", 0x100, 0x124, [], [], variables=[a, b])])
    groups = [bss.Group([[a]], "src/x"), bss.Group([[b]], "src/x")]
    bss.add_tails(module, groups)
    check("add_tails: the slice before ends at the aligned variable", groups[0].end == 0x120, hex(groups[0].end))
    groups = [bss.Group([[b]], "src/x")]
    try:
        bss.add_tails(module, groups)
        check("add_tails: stops when no planned file ends there", False)
    except SystemExit as e:
        check("add_tails: stops when no planned file ends there", "b at 0x120" in str(e), e)


def test_range_counts() -> None:
    # Two gaps; the range takes the second. Its sizes go 8, 4, 8: two runs.
    items1 = [bss.Var(0x100, 8, 8, "a"), bss.Var(0x108, 4, 4, "b"), bss.Var(0x10c, 4, 4, "c")]
    items2 = [bss.Var(0x200, 8, 8, "d"), bss.Var(0x208, 4, 4, "e"), bss.Var(0x20c, 8, 8, "f")]
    module = fake_module([bss.Gap(".bss", 0x100, 0x110, items1, []), bss.Gap(".bss", 0x200, 0x214, items2, [])])
    module.index.functions = []
    plan = bss.make_plan(module, 0x200, 0x300)
    check("make_plan --range: runs of the range only", (plan.runs_before, plan.runs_after) == (2, 2),
          (plan.runs_before, plan.runs_after))
    check("make_plan: one new file per run", [(g.start, g.end) for g in plan.groups] == [(0x200, 0x208), (0x208, 0x214)],
          [(hex(g.start), hex(g.end)) for g in plan.groups])


def test_take_definitions() -> None:
    text = ('#include "types.h"\n\n/* The first. */\nu8 data_a[4];\nstatic u16 data_b;\n'
            'extern u32 data_c;\n\nvoid f(void) {\n  data_b = 1;\n}\n')
    rest, at, blocks = bss.take_definitions(text, ["data_b", "data_a"])
    check("take_definitions: blocks", blocks == {"data_a": "/* The first. */\nu8 data_a[4];",
                                                 "data_b": "static u16 data_b;"}, blocks)
    check("take_definitions: the rest", rest == '#include "types.h"\n\nextern u32 data_c;\n\nvoid f(void) {\n'
          '  data_b = 1;\n}\n' and at == len('#include "types.h"\n\n'), (rest, at))


GEN_RUN = """
import sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
import bss
bss.ROOT = Path(sys.argv[2])
bss.gen_files(Path(sys.argv[3]))
"""


def make_gen_config(root: Path, header: str) -> None:
    (root / "config/AWRE/arm9/overlays").mkdir(parents=True)
    (root / "config/AWRE/arm7").mkdir(parents=True)
    (root / "config/AWRE/arm7/delinks.txt").write_text("")
    (root / "config/AWRE/arm9/delinks.txt").write_text(
        "    .bss        start:0x02100000 end:0x02100100\n\n"
        "src/x/code.c:\n    complete\n    .bss        start:0x02100000 end:0x02100004\n\n"
        "src/x/x_bss.c:\n    complete\n    .bss        start:0x02100004 end:0x02100060\n")
    (root / "config/AWRE/arm9/symbols.txt").write_text(
        "data_02100000 kind:bss(size=0x4) addr:0x02100000\n"
        "data_02100004 kind:bss(size=0x4) addr:0x02100004\n"
        "gTestTable kind:bss(size=0x6) addr:0x02100008\n"
        "data_0210000a kind:bss(size=0x0) addr:0x0210000a\n"
        "data_02100010 kind:bss(size=0x8) addr:0x02100010\n"
        "data_02100018 kind:bss(size=0x8) addr:0x02100018\n"
        "data_02100040 kind:bss(size=0x20) addr:0x02100040\n")
    (root / "config/AWRE/bss_gen.txt").write_text("src/x/x_bss.c\n")
    (root / "include").mkdir()
    (root / "include/test_gen.h").write_text(header)


def run_gen(root: Path, out: Path, seed: str) -> str:
    r = subprocess.run([sys.executable, "-c", GEN_RUN, str(ROOT / "tools"), str(root), str(out)],
                       capture_output=True, text=True, env={**os.environ, "PYTHONHASHSEED": seed})
    if r.returncode:
        return "error: " + r.stderr
    return (out / "bss/x_bss.inc.c").read_text()


def test_gen() -> None:
    with tempfile.TemporaryDirectory(prefix="bss_gen") as d:
        root, out = Path(d) / "repo", Path(d) / "out"
        make_gen_config(root, "extern u16 gTestTable[];\n")
        first = run_gen(root, out, "1")
        second = run_gen(root, out, "2")
        shutil.rmtree(out)
        third = run_gen(root, out, "1")
        check("gen: same text under two hash seeds and from scratch", first == second == third, (first, second, third))
        check("gen: header type, aligned variable, u8 for the rest", all(line in first for line in (
            '#include "test_gen.h"', "u16 gTestTable[0x3];", "u8 data_02100018[0x8];",
            "__attribute__((aligned(32))) u8 data_02100040[0x20];")), first)
        (root / "include/test_gen.h").write_text("extern u16 gTestTable[];\nextern u32 data_02100010[2];\n")
        typed = run_gen(root, out, "1")
        changed = sorted(set(typed.splitlines()) ^ set(first.splitlines()))
        check("gen: a header type changes only that line", changed == ["u32 data_02100010[2];",
                                                                       "u8 data_02100010[0x8];"], changed)
        if not pragmas.CC.exists():
            return
        names = [("data_02100004", 4), ("gTestTable", 6), ("data_02100010", 8), ("data_02100018", 8),
                 ("data_02100040", 0x20)]
        g = bss.Group([[bss.Var(0, bss.round4(s), s, n, align32=n == "data_02100040") for n, s in names]], "src/x",
                      path="src/x/x_bss.c")
        elf = bss.compile_probe(str(root / "include/probe.c"), typed)
        check("gen: the generated text compiles", elf is not None, bss.compile_errors)
        if elf is not None:
            errors = bss.order_errors(elf, g)
            check("gen: variables come out in address order, aligned", not errors, errors)


class NoDecls:
    def of(self, name: str) -> list:
        return []


class FakeIndex:
    decls = NoDecls()
    functions: list = []


def compile_runs(what: str, sizes: list[list[int]], path: str, section: str) -> None:
    """Writes a file of runs of these sizes with body_lines, compiles it as
    `path` (src/arm7/ gets the ARM7 flags) and checks each run's section."""
    runs, addr = [], 0
    for run in sizes:
        runs.append([])
        for s in run:
            runs[-1].append(bss.Var(addr, bss.round4(s), s, f"v_{addr:03x}"))
            addr += bss.round4(s)
    g = bss.Group(runs, str(Path(path).parent), path=path, section=section)
    lines, headers = bss.body_lines(FakeIndex(), g, set())
    elf = bss.compile_probe(g.path, bss.new_file_text(lines, headers, True))
    check(f"{what} compile", elf is not None, bss.compile_errors)
    if elf is not None:
        errors = bss.order_errors(elf, g)
        check(f"{what} come out in order", not errors, errors)


def test_compile() -> None:
    if not pragmas.CC.exists():
        print("test_bss: no compiler, run make setup (compile test skipped)")
        return
    compile_runs("three runs", [[4, 4, 8], [4, 0x10], [8, 8]], "src/x/bss_test.c", ".bss")
    compile_runs("ARM7: two .sbss runs", [[2, 0xc], [8]], "src/arm7/x/sbss_test.c", ".sbss")
    compile_runs("ARM7: three .bss runs", [[4, 0x54, 0x78], [4, 0x24], [0x10, 0x10]], "src/arm7/x/bss_test.c", ".bss")
    compile_aligned()


class PointerDecls:
    """Declares v_ptr as a pointer, so that its definition is one."""

    def of(self, name: str) -> list:
        if name != "v_ptr":
            return []
        return [bss.Decl("v_ptr", "src/x/bss_test.c", 0, 0, "void* v_ptr", "")]


class PointerIndex:
    decls = PointerDecls()


def compile_aligned() -> None:
    """A pointer and a u8 array aligned to 32, one in .bss and one in
    .bss.run1: both sections must have alignment 32."""
    runs = [[bss.Var(0, 4, 4, "v_a"), bss.Var(0x20, 4, 4, "v_ptr", align32=True)],
            [bss.Var(0x40, 0x10, 0x10, "v_buf", align32=True)]]
    g = bss.Group(runs, "src/x", host="src/x/bss_test.c", path="src/x/bss_test.c")
    lines, headers = bss.body_lines(PointerIndex(), g, set())
    text = bss.new_file_text(lines, headers, True)
    check("aligned: the attribute goes in front", "__attribute__((aligned(32))) void* v_ptr;" in text, text)
    elf = bss.compile_probe(g.path, text)
    check("aligned: compile", elf is not None, bss.compile_errors)
    if elf is None:
        return
    check("aligned: sections aligned to 32", not bss.order_errors(elf, g), bss.order_errors(elf, g))


def test_twice() -> None:
    if not (bss.FNINDEX.exists() and bss.DSINDEX.exists() and pragmas.CC.exists()):
        print("test_bss: no build/fnindex.json, build/dsindex.json or compiler (plan test skipped)")
        return
    for module in bss.module_modules():
        cmd = [sys.executable, "tools/bss.py", "plan", module, "--show"]
        first, second = (subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True,
                                        env={**os.environ, "PYTHONHASHSEED": seed}) for seed in ("1", "2"))
        # A plan that stops on aliases past a declared size lists them; that
        # list must be the same too.
        ok = first.returncode == 0 or "past the declared size" in first.stderr
        check(f"plan {module} --show runs", ok, first.stderr.strip())
        check(f"plan {module} --show gives the same output under two hash seeds", first.stdout == second.stdout)


def main() -> int:
    test_items()
    test_past_aliases()
    test_best_host()
    test_slice_run()
    test_tails()
    test_range_counts()
    test_take_definitions()
    test_gen()
    test_filler_cuts()
    test_split_filler()
    test_runs()
    test_link_order()
    test_lcf()
    test_sections()
    test_segments()
    test_compile()
    test_twice()
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
