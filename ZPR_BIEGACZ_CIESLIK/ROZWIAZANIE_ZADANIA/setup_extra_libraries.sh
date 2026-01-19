#!/bin/bash
set -e

PROJECT_DIR=$(pwd)
GTEST_DIR="$PROJECT_DIR/libs/googletest"

echo "=== Project extra libraries setup ==="
echo
echo "Updating package list..."
sudo apt-get update
echo
echo "I. Installing Build Tools (CMake, Make, GCC)..."
if command -v apt-get &> /dev/null; then
    echo "Installing: build-essential, cmake..."
    sudo apt-get install -y build-essential cmake
    echo "Build tools installed successfully."
else
    echo
    echo "WARNING: 'apt-get' not found. Skipping build tools installation."
    echo "Please ensure 'cmake', 'make', 'g++' (build-essential) are installed."
fi

echo
echo "II. Downloading Qt5..."
if command -v apt-get &> /dev/null; then
    sudo apt-get install -y qtbase5-dev qt5-qmake qtbase5-dev-tools
else
    echo
    echo "WARNING: 'apt-get' not found. Skipping automatic Qt5 installation."
    echo "Please ensure 'qtbase5-dev' and build tools are installed manually."
fi


echo
echo "III. Configuring Google Test..."
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

echo
echo "IV. Installing Doxygen (for documentation generation)..."
if command -v apt-get &> /dev/null; then
    sudo apt-get install -y doxygen graphviz
    echo "Doxygen and Graphviz installed successfully!"
fi

echo
echo "=== Setup process finished! ==="
