@echo off
g++ -static main.cc -std=c++17 -O2 -s -march=haswell -fno-exceptions -flto ^
 -lglfw3 -lopengl32 -lglu32 -lgdi32
