del SolarSys.exe
g++ -o SolarSys ./src/SolarSys.cpp -I ./include/ -std=c++14 -D FREEGLUT_STATIC -L"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\lib" -lfreeglut_static -lglu32 -lopengl32 -lwinmm -lgdi32 -Wl,--subsystem,windows -g
SolarSys.exe
