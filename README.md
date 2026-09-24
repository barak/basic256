# BASIC256

[![Build](https://img.shields.io/github/actions/workflow/status/uglymike17/basic256/build.yml?label=CI)](https://github.com/uglymike17/basic256/actions)
[![Latest release](https://img.shields.io/github/v/release/uglymike17/basic256?include_prereleases)](https://github.com/uglymike17/basic256/releases)
[![License: GPLv3](https://img.shields.io/badge/license-GPLv3-blue)](license.txt)

> **BASIC256 is a small, approachable language that lets a beginner gradually grow into graphics, simulation, games and systems programming.**

<p align="center">
  <img src="resources/icons/basic256_256.png" width="160" alt="BASIC256 logo">
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  <img src="BitBot_Hello.png" height="160" alt="BitBot, the BASIC256 mascot">
</p>

BASIC256 is a free, open-source BASIC programming environment for beginners and hobbyists. It is the actively maintained continuation of the original BASIC256, modernized for **Windows, Linux, macOS and the Web** while retaining compatibility with existing BASIC256 programs.

- 🌐 [Homepage](https://basic256.org)
- 📚 [Documentation](https://doc.basic256.org)
- ▶️ [Run BASIC256 in your browser](https://run.basic256.org)
- 💾 [Download releases](https://github.com/uglymike17/basic256/releases)

## Why BASIC256?

- Designed specifically for beginners and hobbyists
- Simple BASIC syntax
- Immediate graphics and sound
- Cross-platform desktop and Web versions
- Many example programs to learn from and experiment with
- Free and open source under GPLv3+

## Try it in your browser

No installation is needed. The WebAssembly version provides the full editor and interpreter directly in your browser.

**[▶ Run BASIC256](https://run.basic256.org)**

For example:

```basic
# bubbles.kbs — random transparent colorful circles
clg
fastgraphics

for i = 1 to 500
    color rgb(int(rand*256), int(rand*256), int(rand*256), 100+int(rand*150))
    circle rand*graphwidth, rand*graphheight, rand*40
    refresh
next i
```

![The bubbles program typed into the BASIC256 editor in a browser tab, with its result in the Graphics Output pane on the right: hundreds of overlapping translucent circles in random colours and sizes filling the canvas](Basic256_in_Browser.png)

The browser version supports several useful URL modes:

| URL parameter | Purpose |
|---|---|
| `?run=name` | Run a bundled Example |
| `?url=path/file.kbs` | Load a program from the site |
| `?src=...` | Load source encoded in the URL |
| `&mode=ide` | Full IDE, auto-run |
| `&mode=edit` | Full IDE, loaded but not run |
| `&mode=graph` | Graphics-only mode |
| `&mode=text` | Text-only mode |
| `&mode=app` | Text + graphics, without the editor |

For example:

- `https://run.basic256.org/?run=Mandelbrot-256&mode=graph`
- `https://run.basic256.org/?run=BubbleUniverse_variations`

This makes the Web version useful not only for learning, but also for sharing and embedding BASIC256 programs.

### Hosting it yourself

Copy the WASM build to any static host served over **HTTPS**, and send these two headers that the multithreaded build relies on:

```
Cross-Origin-Opener-Policy: same-origin
Cross-Origin-Embedder-Policy: require-corp
```

With those in place the page loads in a single pass — no reload — and the bundled `coi-serviceworker` helper (only needed because GitHub Pages can't send those headers itself) is no longer required.

### Browser differences

The browser runs inside a sandbox, so some desktop features are unavailable:

- Files created by a running program exist only for the current browser session.
- Programs in the editor persist across refreshes.
- `SOUND`, `SAY` and related features use browser audio/speech support and could be unavailable.
- TCP networking, `SYSTEM`, serial ports, database/SQL and printer commands are unavailable.
- Large fractals and simulations generally run slower than on the desktop.
- Media such as `SOUNDLOAD` and `IMGLOAD` can be loaded from paths relative to the Web page, subject to normal browser CORS rules.

## What's new

### BASIC256 2.3.0

- Safer permissions for operations involving files, folders, network connections and SQLite, reducing the ability of programs to modify the host system unexpectedly.
- Retro-style console features: `LOCATE`, `TEXTCOLOR`, `TEXTBACKGROUND`, `TEXTFONT`, `TEXTCOL` and `TEXTROW`. (see Examples/Console)
- Printing at a `LOCATE` position overwrites the existing text rather than inserting it.

### BASIC256 2.2.0

- `WINDOW` for logical canvas coordinates.
- Drawing and location-based commands support `WINDOW` coordinates and fractional positions.
- `NOISE` provides OpenSimplex noise.
- `MAT` adds matrix operations: `MUL`, `ADD`, `SUB`, `INV` and `TRN`.
- Vector operations: `DOT`, `CROSS`, `NORM` and `UNIT`.
- `FRAMERATE` for steady drawing-loop timing.
- A documented `turtle.kbs` module for turtle graphics.
- Multi-line array/map literals and comments inside literals.
- Programs run 20-25% faster than 2.1.1, and none run slower.
- Arrays use about half the previous memory, while whole-array operations such as `DIM`, `REDIM` and `MAT` are substantially faster.
- More accurate, interruptible `PAUSE`.
- Updated examples, including examples in the WASM version.
- Various bug fixes, including `MOD` recognition and Windows graphics/mouse behavior.

### BASIC256 2.1.1

- About 2× faster arithmetic-heavy loops.
- Persistent WebAssembly editor state across browser refreshes.

### BASIC256 2.1

- CMake / GitHub Actions build system and Qt6 support.
- WebAssembly support.
- Native macOS support for Apple Silicon and Intel Macs.
- Command-line modes for graphics, text, fullscreen and silent execution.
- Dark themes and updated examples.
- New standard library.
- Updated Docusaurus documentation.

## Desktop application

The desktop application provides the familiar BASIC256 three-pane environment:

- **Editor** — write and edit BASIC programs.
- **Text Output** — program output and console-style interaction.
- **Graphics Output** — immediate drawing and animation.

The same BASIC256 programs can therefore be developed on the desktop and shared through the Web version.

![The BASIC256 desktop IDE on Windows running the Bubble Universe demo, with the syntax-highlighted source on the left and the Text Output and Graphics Output panes on the right](Basic256-IDE.png)

## Download & install

Get the latest builds from the **[Releases](https://github.com/uglymike17/basic256/releases)** page.

| Platform | Status | Notes |
|---|:---:|---|
| Windows | ✅ | ZIP and installer builds available. SmartScreen will initially block the installer as it comes from an unknown source — "More info" → "Run anyway" fixes this. |
| Linux x86 | ✅ | Tarball, AppImage and `.deb` packages. BASIC256 2.1.1 is in Debian 14 "Forky" main and in Debian Unstable "sid" main. |
| Raspberry Pi | ✅ | Tarball and AppImage builds. BASIC256 2.1.1 is in Raspbian Testing main. |
| macOS Apple Silicon | ⚠️ | Requires macOS 15 (Sequoia) or newer. |
| macOS Intel | ⚠️ | Requires macOS 15 (Sequoia) or newer. Separate x86_64 build. |
| WebAssembly | 🧪 | Usable, with the browser limitations described above. |

### macOS

The supplied macOS applications are ad-hoc signed rather than notarized. If Gatekeeper reports that the application is damaged, try:

```console
xattr -cr /Applications/basic256.app
```

For older macOS versions, the original Qt5-based BASIC-256 2.0.0.11 may be an alternative, or BASIC256 can be built from source against Qt 5.15.

### Raspberry Pi

The Raspberry Pi build is intended for modern Raspberry Pi Linux installations. Speech support may require `speech-dispatcher` to be installed separately.

## Command line

BASIC256 can also run programs from a terminal:

| Short | Long | Effect |
| :---: | :--- | :--- |
| -h (-? on Windows) | --help | Display command-line help. |
| | --help-all | Display command-line help including Qt-specific options. |
| -v | --version | Display the BASIC-256 version. |
| -r | --run | Load and run the specified .kbs program. Must precede the filename. |
| -a | --app --application | Load and run the specified .kbs without the Edit window. |
| -g | --graph | Load and run the specified .kbs with only the Graphics window. |
| -t | --text | Load and run the specified .kbs with only the Text window. |
| -f | --full | When used with -r/-a/-t/-g, the full screen area will be used. |
| -s | --silent | Run the specified .kbs with no GUI at all: PRINT goes to stdout, errors to stderr, and the exit code says whether it worked. Needs a filename, and cannot be combined with -r/-a/-g/-t. |
| -l | --lang --language | Start BASIC-256 using the specified language. |

For example:

```console
basic256 -g Mandelbrot-256.kbs
basic256 -f -t Zork256.kbs
basic256 -s TestProgram.kbs
```

## Examples

The repository contains a large collection of example programs covering graphics, games, mathematics, simulations, sound and other BASIC256 features.

The original minimal examples from BASIC256 2.0.0.11 are preserved under:

```text
Examples/Original_Examples/
```

Additional examples can be found at [Manuel Santos' BASIC256 blog](https://basic256.blogspot.com/).

## Standard library

BASIC256 includes 2 small standard libraries:

1. Modules/math.kbs

Use it with:

```basic
include "math.kbs"
```

It currently provides functions including:

`minarr`, `maxarr`, `sumarr`, `avgarr`, `sign`, `min`, `max`, `lerp`, `hypot`, `atan2`, `clamp`, `remap`, `wrap`, `dist`, `fmod`, `fround`, `cbrt`, `randint` and `gaussian`.

These are implemented as BASIC256 code rather than built-in language commands, keeping the language itself small while making common operations convenient.

2. Modules/turtle.kbs

Use it with:

```basic
include "turtle.kbs"
```

This provides turtle logic with following subroutines:  

`t_reset`, `t_goto`, `t_home`, `t_forward`, `t_backward`, `t_right`, `t_left`, `t_setheading`, `t_penup`, `t_pendown`, `t_x`, `t_y`, `t_getheading`, `t_getpen`  
and short-codes `t_fw`, `t_bw`, `t_pu`, `t_pd`, `t_r`, `t_l`

## Build from source

Detailed build instructions are in:

- [COMPILING.txt](COMPILING.txt)
- [COMPILING_RaspberryPI.txt](COMPILING_RaspberryPI.txt)

The project uses **CMake, Qt6 and GitHub Actions**, with builds for Windows, Linux, Raspberry Pi, macOS and WebAssembly.

## Project history

BASIC256 began as **KidBasic** in 2006, created by Ian Paul Larsen and later maintained by James Reneau and other contributors through SourceForge. The original project eventually became BASIC256 and reached version 2.0.0.11.

This repository is a continuation of that project. It started from the 2.0.99.10.2 development branch with the goal of modernizing the codebase while preserving its educational character and compatibility.

The major development steps have been:

- **2.1.0** — modern build system, portability and new platforms.
- **2.1.1** — significant performance improvements and Web persistence.
- **2.2.0** — new language features, mathematics, graphics capabilities and further performance improvements.
- **2.3.0** — safer system access and enhanced text-console capabilities.

The original copyright notices have been retained.

## Roadmap

Development continues with an emphasis on **education, portability, performance and compatibility**.

Current areas of interest include:

- More distribution packages
- More standard modules
- More and better examples
- Educational tutorials
- New language features where a module would be too slow
- Continued improvements to documentation and deployment

## Contributing

BASIC256 is a hobbyist/open-source project and contributions are welcome.

You can help by:

- Reporting bugs
- Improving documentation
- Writing examples
- Translating documentation
- Testing releases
- Improving tutorials
- Submitting pull requests

Use:

- [Issues](https://github.com/uglymike17/basic256/issues) for bugs and feature requests
- [Discussions](https://github.com/uglymike17/basic256/discussions) for questions, ideas and projects
- [Discord](https://discord.gg/8QaSGYAQ9R) to join the community

## License

BASIC256 is distributed under the **GNU General Public License version 3 or later (GPLv3+)**.

The original project used GPLv2 "or (at your option) any later version", allowing this continuation to move to GPLv3+. See [license.txt](license.txt) for the full license.

Two components retain their compatible original licenses:

- `src/core/md5.cpp` / `md5.h` — RSA Data Security code adapted by Frank Thilo
- `src/gui/LineNumberArea.cpp` / `.h` — BSD-licensed code from the Qt examples

## Maintainer

BASIC256 is maintained as a hobbyist/open-source project with the help of modern AI development tools. Contributions to the code, examples, documentation and translations are especially welcome.
