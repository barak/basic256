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
# 15.0 matches what the app actually needs. Qt's own libraries declare 14.0,
# but macdeployqt also copies in Homebrew dependencies built for the runner
# image -- libpcre2 and the virtual-keyboard plugin both require 15 -- and the
# bundle is only as portable as its least portable library. Declaring 14 here
# would give an app that launches on 14 and then dies when dyld reaches one of
# those, which is a worse failure than an honest "requires macOS 15".
#
# 15 is also the practical floor: GitHub's oldest Intel image is macos-15, so
# the Intel build cannot be produced against anything older. Lowering this means
# moving both builds to an older image AND checking what macdeployqt pulls in --
# see the bundled-library check at the end of package_macos.sh.
MACOS_DEPLOYMENT_TARGET="${MACOS_DEPLOYMENT_TARGET:-15.0}"

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

    APP_MIN="$(vtool -show-build "${BIN}" 2>/dev/null | awk '/minos/{print $2; exit}')"
    echo "==> Binary minimum macOS: ${APP_MIN:-unknown}"
fi

# NOTE: the bundled *libraries* are deliberately checked in package_macos.sh
# instead of here. macdeployqt has not run at this point, so the bundle holds
# only our own binary -- scanning for Qt libraries here finds nothing and
# reports a reassuring "unknown" rather than the truth.
