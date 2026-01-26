#!/bin/bash
set -e

# 项目根目录
ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"

GENERATED_DIR="$ROOT_DIR/generated"
ANTLR_CACHE_DIR="$ROOT_DIR/ANTLR/.antlr"
BUILD_DIR="$ROOT_DIR/build"
BIN_DIR="$ROOT_DIR/bin"

clean() {
    echo "==> Cleaning project..."

    rm -rf "$GENERATED_DIR"
    rm -rf "$ANTLR_CACHE_DIR"
    rm -rf "$BUILD_DIR"
    rm -rf "$BIN_DIR"

    echo "==> Clean completed."
}

build() {
    echo "==> Generating sources..."
    rm -rf "$GENERATED_DIR"
    bash "$ROOT_DIR/generate.sh"

    echo "==> Preparing build directory..."
    rm -rf "$ANTLR_CACHE_DIR"
    rm -rf "$BUILD_DIR"
    mkdir -p "$BUILD_DIR"

    echo "==> Configuring with CMake..."
    cd "$BUILD_DIR"
    cmake ..

    echo "==> Building..."
    make
}

rebuild() {
    clean
    build
}

# 参数分发
case "$1" in
    clean)
        clean
        ;;
    rebuild)
        rebuild
        ;;
    ""|build)
        build
        ;;
    *)
        echo "Usage: $0 [build|clean|rebuild]"
        exit 1
        ;;
esac
