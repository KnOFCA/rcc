#!/bin/bash
set -e

# 项目根目录
ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"

GENERATED_DIR="$ROOT_DIR/generated"
ANTLR_CACHE_DIR="$ROOT_DIR/ANTLR/.antlr"
BUILD_DIR="$ROOT_DIR/build"
BIN_DIR="$ROOT_DIR/bin"

# 记录重要文件的修改时间
TRACKED_FILES=(
    "$ROOT_DIR/ANTLR/*.g4"
    "$ROOT_DIR/generate.sh"
    "$ROOT_DIR/CMakeLists.txt"
    "$ROOT_DIR/src/*.cpp"
    "$ROOT_DIR/src/*.h"
    "$ROOT_DIR/include/*.h"
)

# 检查是否需要重新生成代码
needs_generation() {
    if [ ! -d "$GENERATED_DIR" ]; then
        return 0  # 需要生成
    fi
    
    # 检查语法文件是否更新
    for pattern in "${TRACKED_FILES[@]}"; do
        for file in $pattern; do
            if [ -f "$file" ]; then
                if [ "$file" -nt "$GENERATED_DIR" ]; then
                    return 0  # 有文件比生成目录新，需要重新生成
                fi
            fi
        done
    done
    
    return 1  # 不需要重新生成
}

clean() {
    echo "==> Cleaning project..."
    
    rm -rf "$GENERATED_DIR"
    rm -rf "$ANTLR_CACHE_DIR"
    rm -rf "$BUILD_DIR"
    rm -rf "$BIN_DIR"
    
    echo "==> Clean completed."
}

# 增量构建
build() {
    echo "==> Checking if generation is needed..."
    
    # 只在必要时重新生成代码
    if needs_generation; then
        echo "==> Generating sources (files have changed)..."
        bash "$ROOT_DIR/generate.sh"
    else
        echo "==> Generation skipped (no changes detected)"
    fi
    
    echo "==> Preparing build directory..."
    
    # 确保构建目录存在
    mkdir -p "$BUILD_DIR"
    
    echo "==> Configuring with CMake..."
    cd "$BUILD_DIR"
    
    # 只在CMakeLists.txt有变化或首次构建时重新配置
    if [ ! -f "CMakeCache.txt" ] || [ "$ROOT_DIR/CMakeLists.txt" -nt "CMakeCache.txt" ]; then
        cmake -DCMAKE_BUILD_TYPE=Debug ..
        echo "==> CMake configuration updated"
    else
        echo "==> Using existing CMake configuration"
    fi
    
    echo "==> Building..."
    
    # 使用所有CPU核心进行构建
    make -j$(nproc)
    
    # 确保bin目录存在
    mkdir -p "$BIN_DIR"
    
    echo "==> Build completed successfully!"
}

# 完全重建（清理后重新构建）
rebuild() {
    clean
    build
}

# 快速构建（仅运行make，假设CMake已配置）
quick() {
    echo "==> Quick build (make only)..."
    
    if [ ! -d "$BUILD_DIR" ]; then
        echo "Error: Build directory not found. Run 'build' first."
        exit 1
    fi
    
    cd "$BUILD_DIR"
    make -j$(nproc)
    echo "==> Quick build completed!"
}

# 参数分发
case "$1" in
    clean)
        clean
        ;;
    rebuild)
        rebuild
        ;;
    quick)
        quick
        ;;
    ""|build)
        build
        ;;
    *)
        echo "Usage: $0 [build|quick|clean|rebuild]"
        echo "  build     - Incremental build (default)"
        echo "  quick     - Make only (fastest)"
        echo "  clean     - Clean all generated files"
        echo "  rebuild   - Clean and rebuild everything"
        exit 1
        ;;
esac
