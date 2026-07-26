#!/usr/bin/env bash
set -euo pipefail

APP_TARGET="basic256"
APP_BUNDLE="build/${APP_TARGET}.app"
ARTIFACT_NAME="${ARTIFACT_NAME:-basic256-MacOS.zip}"

# Architecture to build for. The matrix passes TARGET_ARCH (arm64 or x86_64);
# falling back to the runner's own architecture keeps the script usable by hand
# on either kind of Mac. Both values are valid CMAKE_OSX_ARCHITECTURES entries.
TARGET_ARCH="${TARGET_ARCH:-$(uname -m)}"

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

echo "==> Configuring CMake for macOS ${TARGET_ARCH}..."
cmake -B build -S . \
    -DCMAKE_PREFIX_PATH="${QT_PREFIX}" \
    -DCMAKE_OSX_ARCHITECTURES="${TARGET_ARCH}" \
    -DCMAKE_BUILD_TYPE=Release

echo "==> Building Project..."
cmake --build build --config Release

# Confirm what actually came out -- a silent arch mismatch here would only show
# up as "app is damaged" on a user's machine.
if [ -x "${APP_BUNDLE}/Contents/MacOS/${APP_TARGET}" ]; then
    echo "==> Built binary architecture: $(lipo -archs "${APP_BUNDLE}/Contents/MacOS/${APP_TARGET}")"
fi
