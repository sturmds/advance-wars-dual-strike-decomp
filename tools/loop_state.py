#!/usr/bin/env python3
"""State of the automated loop (docs/loop.md, .claude/skills/decomp-orchestrate).

The state lives outside git, in $LOOP_DIR (default
work/loop): state.json, journal.md, steer.md. Agents in
worktrees never read or write it, so it cannot touch landing.

    loop_state.py lease                 take or refresh the orchestrator lease
    loop_state.py add-batch N DIR       queue DIR/plan.json's groups as batch N
    loop_state.py add-job NAME PROMPT --model M [--after A,B] [--class C]
    loop_state.py reconcile             count each group's functions now in C
    loop_state.py status [--all]        one line per open group, then totals
    loop_state.py next K                up to K queued groups ready to start
    loop_state.py set NAME key=value... (tokens+=N adds; tries+=1 counts)
    loop_state.py claimed               functions held by open groups
    loop_state.py report [N]            functions and tokens per batch
    loop_state.py log TEXT              append a line to journal.md

A group is one agent's work: a plan.json entry (with functions) or a job
(planner, partial retry, renames, tool fix) with only a prompt. Statuses:
queued, running, stopped (usage limit or lost session: resume or relaunch),
done, partial, failed. Functions count as done by address, from the
`complete` files in delinks.txt, so renames do not matter.
"""

import argparse
import fcntl
import json
import os
import sys
import time
from contextlib import contextmanager
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fn  # noqa: E402

LOOP_DIR = Path(os.environ.get("LOOP_DIR", Path("work/loop")))
STATE = LOOP_DIR / "state.json"
OPEN = ("queued", "running", "stopped")
FINISHED = ("done", "partial", "failed")


def now() -> str:
    return time.strftime("%Y-%m-%d %H:%M")


@contextmanager
def state(write: bool = True):
    LOOP_DIR.mkdir(parents=True, exist_ok=True)
    with open(LOOP_DIR / ".lock", "w") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        s = json.loads(STATE.read_text()) if STATE.exists() else {"lease": None, "seq": 0, "groups": {}}
        yield s
        if write:
            tmp = STATE.with_suffix(".tmp")
            tmp.write_text(json.dumps(s, indent=1) + "\n")
            tmp.replace(STATE)


def session() -> tuple[str, int]:
    return os.environ.get("CLAUDE_CODE_SESSION_ID", "unknown"), int(os.environ.get("CLAUDE_PID", "0"))


def alive(pid: int) -> bool:
    if pid <= 0:
        return False
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return False
    except PermissionError:
        pass
    return True


def done_addrs() -> set[tuple[str, int]]:
    out = set()
    for mod in fn.modules().values():
        spans = [(s, e) for _, s, e, complete in mod.files if complete]
        for f in mod.funcs:
            if any(s <= f.addr < e for s, e in spans):
                out.add((mod.name, f.addr))
    return out


def cmd_lease(a, s):
    sid, pid = session()
    held = s.get("lease")
    if held and held["session"] != sid and alive(held["pid"]) and not a.force:
        print(f"HELD by session {held['session']} (pid {held['pid']}, since {held['since']}); not taking it")
        return 1
    if not held or held["session"] != sid:
        s["lease"] = {"session": sid, "pid": pid, "since": now()}
        # Agents started by an earlier session cannot be resumed from this one.
        lost = [n for n, g in s["groups"].items() if g["status"] == "running" and g.get("session") != sid]
        for n in lost:
            s["groups"][n]["status"] = "stopped"
            s["groups"][n]["lost"] = True
        print(f"TOOK lease for {sid}; {len(lost)} running groups from earlier sessions marked stopped (lost): {' '.join(lost)}")
    else:
        print(f"OK lease held by this session since {held['since']}")
    s["lease"]["seen"] = now()
    return 0


def add_group(s, name, **fields):
    if name in s["groups"]:
        sys.exit(f"{name}: already in the state")
    s["seq"] += 1
    g = {"status": "queued", "tries": 0, "tokens": 0, "order": s["seq"], "after": [], "cls": None,
         "agent": None, "session": None, "funcs": [], "done": 0, "started": None, "ended": None, "result": ""}
    g.update(fields)
    s["groups"][name] = g


def cmd_add_batch(a, s):
    d = Path(a.dir).expanduser().resolve()
    plan = json.loads((d / "plan.json").read_text())["groups"]
    order = [ln.split()[0] for ln in (d / "order.txt").read_text().splitlines()
             if ln.strip() and not ln.startswith("#") and not ln.endswith(":")] if (d / "order.txt").exists() else []
    names = [n for n in order if n in plan] + sorted(n for n in plan if n not in order)
    for n in names:
        g = plan[n]
        add_group(s, f"b{a.n}/{n}", batch=a.n, model=g["model"], prompt=str(d / f"{n}.md"),
                  funcs=[[f[0], f[1], f[3]] for f in g["funcs"]], after=g.get("after", []), cls=g.get("cls"))
    print(f"queued {len(names)} groups of batch {a.n}")
    return 0


def cmd_add_job(a, s):
    add_group(s, a.name, batch=a.batch, model=a.model, prompt=str(Path(a.prompt).expanduser().resolve()),
              after=[x for x in (a.after or "").split(",") if x], cls=a.cls)
    print(f"queued job {a.name}")
    return 0


def cmd_reconcile(a, s):
    done = done_addrs()
    for n, g in s["groups"].items():
        if g["funcs"]:
            g["done"] = sum((m, addr) in done for m, addr, _ in g["funcs"])
            if g["status"] in ("queued", "stopped") and g["done"] == len(g["funcs"]):
                g["status"], g["result"] = "done", "all functions in C (reconcile)"
    return cmd_status(a, s)


def cmd_status(a, s):
    rows = [(n, g) for n, g in sorted(s["groups"].items(), key=lambda x: x[1]["order"])
            if a.all or g["status"] in ("running", "stopped")]
    for n, g in rows:
        fns = f"{g['done']}/{len(g['funcs'])}" if g["funcs"] else "job"
        lost = " lost" if g.get("lost") else ""
        print(f"{n:28} {g['status']:8}{lost} {g['model']:6} {fns:>7} tries={g['tries']} agent={g['agent']} {g['result'][:60]}")
    counts = {}
    for g in s["groups"].values():
        counts[g["status"]] = counts.get(g["status"], 0) + 1
    print("totals:", " ".join(f"{k}={v}" for k, v in sorted(counts.items())), "| ready:", len(ready(s)))
    return 0


def ready(s):
    st = {n: g["status"] for n, g in s["groups"].items()}
    busy = {g["cls"] for g in s["groups"].values() if g["status"] == "running" and g["cls"]}
    out = []
    for n, g in sorted(s["groups"].items(), key=lambda x: (x[1].get("batch") or 0, x[1]["order"])):
        if g["status"] != "queued" or any(st.get(d) not in FINISHED for d in g["after"]):
            continue
        if g["cls"] and g["cls"] in busy:
            continue
        if g["cls"]:
            busy.add(g["cls"])
        out.append(n)
    return out


def cmd_next(a, s):
    for n in ready(s)[: a.k]:
        g = s["groups"][n]
        print(f"{n}\t{g['model']}\t{g['prompt']}")
    return 0


def cmd_set(a, s):
    g = s["groups"].get(a.name) or sys.exit(f"{a.name}: not in the state")
    for kv in a.pairs:
        if "+=" in kv:
            k, v = kv.split("+=", 1)
            g[k] = g.get(k, 0) + int(v)
            continue
        k, v = kv.split("=", 1)
        g[k] = v
        if k == "status" and v == "running":
            g["started"], g["session"] = now(), session()[0]
            g.pop("lost", None)
        if k == "status" and v in FINISHED:
            g["ended"] = now()
    return 0


def cmd_claimed(a, s):
    for n, g in s["groups"].items():
        if g["status"] in OPEN:
            for m, addr, name in g["funcs"]:
                print(f"{m}\t0x{addr:08x}\t{name}\t{n}")
    return 0


def cmd_report(a, s):
    done = done_addrs()
    by = {}
    for g in s["groups"].values():
        b = by.setdefault(g.get("batch"), {"groups": 0, "open": 0, "funcs": 0, "done": 0, "tokens": 0})
        b["groups"] += 1
        b["open"] += g["status"] in OPEN
        b["funcs"] += len(g["funcs"])
        b["done"] += sum((m, addr) in done for m, addr, _ in g["funcs"])
        b["tokens"] += g["tokens"]
    for b, r in sorted(by.items(), key=lambda x: (x[0] is None, x[0] or 0)):
        if a.n is not None and b != a.n:
            continue
        rate = r["done"] / r["tokens"] * 1e6 if r["tokens"] else 0
        print(f"batch {b}: groups {r['groups']} (open {r['open']}), functions {r['done']}/{r['funcs']}, "
              f"tokens {r['tokens']:,}, {rate:.0f} functions per 1M tokens (agent-matched only)")
    return 0


def cmd_log(a, s):
    with open(LOOP_DIR / "journal.md", "a") as f:
        f.write(f"- {now()} {a.text}\n")
    return 0


def main() -> int:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)
    sub.add_parser("lease").add_argument("--force", action="store_true")
    x = sub.add_parser("add-batch")
    x.add_argument("n", type=int)
    x.add_argument("dir")
    x = sub.add_parser("add-job")
    x.add_argument("name")
    x.add_argument("prompt")
    x.add_argument("--model", required=True)
    x.add_argument("--batch", type=int)
    x.add_argument("--after")
    x.add_argument("--class", dest="cls")
    sub.add_parser("reconcile").add_argument("--all", action="store_true")
    sub.add_parser("status").add_argument("--all", action="store_true")
    sub.add_parser("next").add_argument("k", type=int)
    x = sub.add_parser("set")
    x.add_argument("name")
    x.add_argument("pairs", nargs="+")
    sub.add_parser("claimed")
    sub.add_parser("report").add_argument("n", type=int, nargs="?")
    sub.add_parser("log").add_argument("text")
    a = p.parse_args()
    handler = globals()["cmd_" + a.cmd.replace("-", "_")]
    with state(write=a.cmd not in ("status", "next", "claimed", "report", "log")) as s:
        return handler(a, s)


if __name__ == "__main__":
    sys.exit(main())
