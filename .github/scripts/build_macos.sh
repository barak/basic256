#!/usr/bin/env bash
set -euo pipefail

APP_TARGET="basic256"
APP_BUNDLE="build/${APP_TARGET}.app"
ARTIFACT_NAME="${ARTIFACT_NAME:-basic256-MacOS.zip}"

# Architecture to build for. The matrix passes TARGET_ARCH (arm64 or x86_64);
# falling back to the runner's own architecture keeps the script usable by hand
# on either kind of Mac. Both values are valid CMAKE_OSX_ARCHITECTURES entries.
TARGET_ARCH="${TARGET_ARCH:-$(uname -m)}"

# Oldest macOS the app should run on. Without this, the minimum is inherited
# from whatever image the build happened on, so it creeps upwards silently
# every time GitHub advances its runner labels.
#
# 14.0 is not arbitrary: it is the floor Homebrew's Qt libraries themselves
# declare, so it is the lowest value we can claim honestly. Going lower would
# produce an app that launches on, say, 13 and then fails when dyld loads a Qt
# library that requires 14. Raise this only together with the runner image.
MACOS_DEPLOYMENT_TARGET="${MACOS_DEPLOYMENT_TARGET:-14.0}"

echo "==> Installing Qt6 via Homebrew..."
brew install qt

# Homebrew's prefix is architecture-dependent -- /opt/homebrew on Apple Silicon,
# /usr/local on Intel -- so ask brew instead of hardcoding either one. This is
# what lets one script serve both macOS matrix entries.
QT_PREFIX="$(brew --prefix qt)"
echo "==> Qt prefix: ${QT_PREFIX}"

# Export into the current shell so the CMake call below finds Qt right now...
export PATH="${QT_PREFIX}/bin:$PATH"

# ...and into GITHUB_PATH so later workflow steps (macdeployqt in the packaging
# script) find it too. Guarded so a local run outside Actions still works.
if [ -n "${GITHUB_PATH:-}" ]; then
    echo "${QT_PREFIX}/bin" >> "$GITHUB_PATH"
fi

echo "==> Configuring CMake for macOS ${TARGET_ARCH} (min ${MACOS_DEPLOYMENT_TARGET})..."
cmake -B build -S . \
    -DCMAKE_PREFIX_PATH="${QT_PREFIX}" \
    -DCMAKE_OSX_ARCHITECTURES="${TARGET_ARCH}" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="${MACOS_DEPLOYMENT_TARGET}" \
    -DCMAKE_BUILD_TYPE=Release

echo "==> Building Project..."
cmake --build build --config Release

# Report what actually came out. Both of these have failed silently before: a
# wrong architecture only shows up as "app is damaged" on a user's machine, and
# a drifting minimum OS only shows up as "requires a newer version of macOS".
BIN="${APP_BUNDLE}/Contents/MacOS/${APP_TARGET}"
if [ -x "${BIN}" ]; then
    echo "==> Built binary architecture: $(lipo -archs "${BIN}")"

    # The app is only as portable as the least portable thing in the bundle, so
    # check the bundled Qt libraries too, not just our own binary.
    APP_MIN="$(vtool -show-build "${BIN}" 2>/dev/null | awk '/minos/{print $2; exit}')"
    echo "==> Binary minimum macOS: ${APP_MIN:-unknown}"

    WORST="$(find "${APP_BUNDLE}" -type f \( -name '*.dylib' -o -path '*.framework/Versions/*' \) \
        -exec vtool -show-build {} \; 2>/dev/null \
        | awk '/minos/{print $2}' | sort -t. -k1,1n -k2,2n | tail -1)"
    echo "==> Highest minimum macOS across bundled libraries: ${WORST:-unknown}"

    if [ -n "${WORST}" ] && [ "${WORST%%.*}" -gt "${MACOS_DEPLOYMENT_TARGET%%.*}" ]; then
        echo "::warning::A bundled library requires macOS ${WORST}, above the" \
             "requested deployment target ${MACOS_DEPLOYMENT_TARGET}. The app will" \
             "in practice need macOS ${WORST}. Homebrew bottles follow the runner" \
             "image, so pin a different macOS image rather than only lowering the" \
             "deployment target."
    fi
fi
