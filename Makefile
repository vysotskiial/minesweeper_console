CXX = g++
CFLAGS = -Wall
LFLAGS = -lGL -lGLU -lglut
OBJS = minesweeper.o mine_board.o
CFLAGS += $(DBG)
all:	minesweeper
 
minesweeper:	$(OBJS)
	$(CXX) minesweeper.o mine_board.o $(LFLAGS) -o minesweeper
	rm *.o

minesweeper.o: minesweeper.cpp
	$(CXX) -c $(CFLAGS) -o $@ $<

mine_board.o: mine_board.cpp
	$(CXX) -c $(CFLAGS) -o $@ $<

clean:
	rm -fr minesweeper.exe *.o
