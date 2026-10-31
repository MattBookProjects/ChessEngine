default: main move piece common
	g++ -o chesseng main.o move.o


main.o: main.cpp
	g++ -c main.o main.cpp


piece: piece.cpp
	g++ -c piece.cpp


move: move.cpp
	g++ -c move.cpp

common: common.cpp
	g++ -c common.cpp



