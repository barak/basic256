# BASIC256 Benchmark

`benchmark.kbs` times each part of the BASIC256 interpreter separately and
prints one table at the end. It is here so that anyone can run it on their own
machine: this folder ships beside the TestSuite in every release.

## Running it

Open `benchmark.kbs` in BASIC256 and press Run. It takes about half a minute on
a current desktop, and the table appears in the text output pane when it
finishes. Nothing is written outside this folder, and the scratch file and
database it makes are deleted before it ends.

From a command line, either of these works:

```
basic256 -r benchmark.kbs      # the IDE, running
basic256 benchmark.kbs         # open it and press Run yourself
```

You can also run it with no windows at all:

```
basic256 -s benchmark.kbs
```

In that mode the output goes to the terminal instead of the pane, which is
convenient for saving a run to a file. Be aware that BASIC256 deliberately
skips all drawing when there is no window, so the **graphics, sprite and text
output lines are meaningless under `-s`** — they report the speed of doing
nothing. Every other line is sound, so `-s` is a fair way to compare two builds
of the interpreter itself.

## Reading the table

```
test                                          operations       ms      per second
-------------------------------------------------------------------------------
integer loop (for/next)                        2,000,000      146      13,698,630
```

* **operations** is how many things that line did -- loop passes, calls,
  characters, cells, rows, whatever the line is counting. It is not the same
  unit from line to line, so compare a line against the same line from another
  run, never against the line below it.
* **ms** is how long that took.
* **per second** is the two divided, as a rate.

The last row is the wall time for the whole run.

## Comparing runs

Only compare runs that used the same `benchscale`. That setting is the first
line of the program:

```basic
benchscale = 1.0
```

Turn it down on a slow machine (a Raspberry Pi, say) and up on a very fast one.
It changes how many times each section repeats, never what a section measures:
the matrix sizes, the sort length and the recursion depth are fixed whatever
the scale.

Two more things worth knowing before reading too much into a number:

* The **graphics, sprite and text output** lines depend on the windowing
  system, the theme and the window size as much as on BASIC256. A run with the
  window at a different size is not comparable.
* The machine should otherwise be idle. A build running in the background will
  show up in the table.

## What is measured

Loops (`for`/`next` and `while`), integer and floating point arithmetic,
modulo, `sin`/`cos`/`sqr`, function calls, subroutine calls, recursion,
one- and two-dimensional array reads and writes, an insertion sort, `MAT`
add/transpose/multiply/invert, the `DOT`/`CROSS`/`NORM`/`UNIT` vector
operators, string concatenation, search, replace, slicing, case conversion and
number conversion, map insert and lookup, `NOISE` in one and two dimensions,
`RAND`, file output and input, database insert and select, `PLOT`/`LINE`/
`RECT`/`CIRCLE`/`STAMP`, `GETSLICE`/`PUTSLICE`, whole-frame drawing with
`REFRESH`, sprite placement, collision and movement, and text output to both
the flowing pane and a `TEXTSCREEN` character grid.

A few lines come in pairs on purpose, because the pair is the interesting part:

* **`for`/`next` against `while`** -- the same count, two loop constructs.
* **graphics drawing against graphics frames** -- the cost of the drawing
  itself, then the cost of showing it with `REFRESH` every pass.
* **database insert committing every row against one transaction** -- a commit
  waits for the disk, so the first of these measures your drive far more than
  it measures BASIC256. The counts differ by a lot for that reason.
* **text output to the flowing pane against a `TEXTSCREEN` grid** -- the
  flowing pane keeps every line it has ever been given and the grid keeps none,
  so they are given different numbers of lines and their rates are separate
  numbers rather than a comparison.
