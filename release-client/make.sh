#!/bin/bash

echo Building file: ../client/main.cpp
echo Invoking: GCC C++ Compiler
g++ -I./../library -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"main.d" -MT"main.d" -o "main.o" "../client/main.cpp" -m32
echo Finished building: ../main.cpp
 
echo Building target: stc
echo Invoking: GCC C++ Linker
g++ -L./../release-library -o "stc"  ./main.o   -lsimple_text_classifier -m32
echo Finished building target: stc
