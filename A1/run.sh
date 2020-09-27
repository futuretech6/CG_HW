if [ ! -d "./bin/" ]; then
    mkdir ./bin/
fi
if [ ! -d "./pic/" ]; then
    mkdir ./pic/
fi
g++ --std=c++14 ./src/A1.cpp -o ./bin/A1.out -g
./bin/A1.out
python3 ./src/convert.py
