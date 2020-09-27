mkdir bin
mkdir pic
g++ --std=c++14 ./src/A1.cpp -o ./bin/A1.exe -g
bin\A1.exe
python src\convert.py
