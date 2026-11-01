#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

APP_ID="text_editor"
WHAT="${1:-all}"
BUILD=build-release
DIST="$PWD/dist"

mkdir -p "$DIST"

cmake -S . -B "$BUILD" -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD" --parallel
VERSION="$(sed -n 's/^CMAKE_PROJECT_VERSION:STATIC=//p' "$BUILD/CMakeCache.txt")"

if [ "$WHAT" = deb ] || [ "$WHAT" = all ]; then
	(cd "$BUILD" && cpack -G DEB)
	cp "$BUILD"/*.deb "$DIST"/
fi

echo "Done. Files in $DIST:"
ls -l "$DIST"
