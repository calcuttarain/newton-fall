#!/bin/bash

OS=$(uname -s)

BOX2D_DIR="externals/Box2D"
BUILD_DIR="build"
LIB_DIR="lib"
INCLUDE_DIR="include"

if [[ "$OS" == "MINGW"* || "$OS" == "CYGWIN"* || "$OS" == "Windows"* ]]; then
  echo "Detected Windows-like OS (Git Bash or WSL)."
  BOX2D_DIR="externals\\Box2D" 
  BUILD_DIR="build"
  LIB_DIR="lib"
  INCLUDE_DIR="include"
fi

if [ ! -d "$BOX2D_DIR" ]; then
  echo "Cloning Box2D..."
  git clone https://github.com/erincatto/box2d.git "$BOX2D_DIR"
else
  echo "Box2D already cloned in $BOX2D_DIR"
fi

if [ ! -d "$BUILD_DIR" ]; then
  mkdir "$BUILD_DIR"
fi

cd "$BOX2D_DIR"

./build.sh

if [ $? -eq 0 ]; then
  echo "Box2D was successfully built!"
else
  echo "Error building Box2D!"
  exit 1
fi
