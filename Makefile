FALGS=-O3 -Werror -lm

game_of_life: game_of_life.c
	cc game_of_life.c $(CFALGS) -o game_of_life


clean:
	rm game_of_life 
