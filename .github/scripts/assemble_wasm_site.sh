#!/usr/bin/env bash
#
# Assemble the publishable WASM site from the Qt build output in build-wasm/
# plus the vendored shell and runtime assets in wasm-deploy/.
#
# This runs in the wasm build job, not at deploy time, so that the uploaded
# BASIC256-WASM artifact *is* the site: unzip it, serve the directory, and you
# are looking at exactly what GitHub Pages would publish. It used to be
# assembled in the pages-deploy job instead, which meant the artifact held
# Qt's bare generated page -- no custom shell, no favicon, no example assets --
# and the only way to see a WASM change was to merge it to main and deploy.
#
# Usage: assemble_wasm_site.sh [output-dir]   (default: public)

set -euo pipefail

OUT="${1:-public}"

rm -rf "$OUT"
mkdir -p "$OUT"

# Qt's build output. Its generated basic256.html is left behind in favour of
# the vendored custom shell -- see wasm-deploy/index.html's header comment for
# why (QTBUG-109959: Qt's CMake build doesn't reliably let you supply a custom
# shell file, so we replace its output post-build instead of fighting the
# build system for it). qtlogo.svg goes with it: the shell shows our own logo.
cp build-wasm/basic256.js   "$OUT/basic256.js"
cp build-wasm/basic256.wasm "$OUT/basic256.wasm"
cp build-wasm/qtloader.js   "$OUT/qtloader.js"

cp wasm-deploy/index.html               "$OUT/index.html"
cp wasm-deploy/coi-serviceworker.min.js "$OUT/coi-serviceworker.min.js"
cp wasm-deploy/favicon.png              "$OUT/favicon.png"
# Splash image on the loading screen, in place of Qt's own qtlogo.svg.
cp wasm-deploy/logo.png                 "$OUT/logo.png"

# Assets that bundled example programs load at runtime. In the browser a
# relative path in imageload/soundload is fetched over HTTP relative to the
# page (MediaPath::downloadUrl), never read from the :/examples resource --
# the qrc only ever supplies .kbs source. So anything an example opens by name
# has to sit next to index.html.
cp wasm-deploy/lady.png               "$OUT/lady.png"   # Image/ImageStatementDemo.kbs
cp wasm-deploy/jim_saying_numbers.mp3 "$OUT/jim_saying_numbers.mp3"

# GitHub Pages runs deployed sites through Jekyll by default, which
# ignores/mangles files and directories starting with an underscore --
# .nojekyll disables that processing.
touch "$OUT/.nojekyll"

echo "== $OUT =="
ls -la "$OUT"
