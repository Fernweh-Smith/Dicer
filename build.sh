#!/bin/sh

SOURCE="./src/main.cpp ./src/dice.cpp"
OUTPUT="./build/dicer"
STANDARD="gnu++20"

clang++ $SOURCE -std=$STANDARD -o $OUTPUT -Wall
