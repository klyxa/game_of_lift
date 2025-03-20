
gcc -pg -O3 game_of_life.c -o game_of_life
./game_of_life test_2.pbm
gprof game_of_life gmon.out > analysis.txt
cat analysis.txt
 
