## Casus Engine
 2D Game Engine Using GL2D And C++

## Note
~~Some of the classes have broken linkages so currently it is not possible to use this repository on personal system~~
Fixed all broken linkages, remake most of the core functional logic so it should run fine now. The main file is now the gamelayer.cpp
 
## Instruction 
1. Clone the git repository
2. Edit the CMakeCache.txt to include the file path as in current system
3. Run the .exe file from the out\build\x64-Debug folder

## Changelogs
1. Changed the core library implementation from SDL2 to GL2D
2. Created a fully playable prototype game, able to be run on Linux, MacOS as well as Windows
3. Changed sprite extration class to be fullfilled by TextureAtlas
