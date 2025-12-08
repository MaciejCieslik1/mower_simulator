#!/bin/bash
set -e

PROJECT_DIR=$(pwd)
GTEST_DIR="$PROJECT_DIR/libs/googletest"
if command -v apt-get &> /dev/null; then
    sudo apt-get update
    sudo apt-get install -y build-essential cmake qtbase5-dev qt5-qmake qtbase5-dev-tools
else
    echo
    echo "WARNING: 'apt-get' not found. Skipping automatic Qt5 installation."
    echo "Please ensure 'qtbase5-dev' and build tools are installed manually."
fi
mkdir -p "$PROJECT_DIR/libs"

if [ -d "$GTEST_DIR" ]; then
    echo "Google Test directory already exists. Skipping clone."
else
    echo "Cloning Google Test..."
    git clone https://github.com/google/googletest.git "$GTEST_DIR"
    echo "Google Test cloned successfully!"
fi

if [ ! -d "$GTEST_DIR/build" ]; then
    echo "Building Google Test library..."
    mkdir -p "$GTEST_DIR/build"
    cd "$GTEST_DIR/build"
    cmake ..
    make
    echo "Google Test built successfully!"
else
    echo "Google Test build directory already exists."
fi