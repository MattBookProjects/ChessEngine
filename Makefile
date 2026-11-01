default: main move piece common chessboard
	g++ -o chesseng main.o move.o piece.o common.o chessboard.o

test: main move piece common chessboard test_common
	g++ -o chesseng main.o move.o piece.o common.o chessboard.o test_common.o

main: main.cpp
	g++ -c main.cpp

chessboard: chessboard.cpp
	g++ -c chessboard.cpp

piece: piece.cpp
	g++ -c piece.cpp


move: move.cpp
	g++ -c move.cpp

common: common.cpp
	g++ -c common.cpp

clean:
	rm *.o

test_common: test_common.cpp
	g++ -c test_common.cpp