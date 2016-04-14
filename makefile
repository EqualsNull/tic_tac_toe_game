CC = g++
FLAG = -std=c++11

tic_tactoe_game : main.o tictactoe.o
	$(CC) $(FLAG)  main.o tictactoe.o -o tic_tactoe_game.exe

main.o : main.cpp tictactoe.cpp tictactoe.h
	$(CC) $(FLAG) -Wall -c main.cpp

tictactoe.o : tictactoe.cpp tictactoe.h
	$(CC) $(FLAG) -Wall -c tictactoe.cpp

clean :
	rm *.o tic_tac_toe_game.exe
