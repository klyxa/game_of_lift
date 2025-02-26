
gcc -pg -O2 game_of_life.c -o game_of_life
./game_of_life
gprof game_of_life gmon.out > analysis.txt
cat analysis.txt
 
