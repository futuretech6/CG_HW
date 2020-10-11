# !/bin/zsh
# GLUT tool-chain installed by `apt install freeglut3-dev`
g++ -o ./bin/Jordan.out ./src/main.cpp -lGL -lglut
./bin/Jordan.out
