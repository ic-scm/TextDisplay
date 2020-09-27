#!/bin/bash
echo "[TextDraw] Building converter..."
g++ -c tocheader.cpp -O2 -Wall `libpng-config --cflags`
g++ tocheader.o -o tocheader `libpng-config --ldflags`
rm tocheader.o

echo "[TextDraw] Building template generator..."
g++ -c gentemplate.cpp -O2 -Wall `libpng-config --cflags`
g++ gentemplate.o -o gentemplate `libpng-config --ldflags`
rm gentemplate.o
