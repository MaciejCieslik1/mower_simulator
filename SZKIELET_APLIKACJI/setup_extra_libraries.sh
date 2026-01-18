#!/bin/bash
set -e

PROJECT_DIR=$(pwd)
GTEST_DIR="$PROJECT_DIR/libs/googletest"

mkdir -p "$PROJECT_DIR/libs"

echo "Clonning Google Test..."
git clone https://github.com/google/googletest.git "$GTEST_DIR"
echo "Google Test successfully cloned!"

mkdir -p "$GTEST_DIR/build"
cd "$GTEST_DIR/build"
cmake ..
make

echo "Configuration finished!"
