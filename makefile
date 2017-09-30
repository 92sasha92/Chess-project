CC = gcc
OBJS = CHMoves.o CHGame.o SimpleButton.o SPSimpleMainWindow.o SPCommon.o gameBoard.o SPArrayList.o CHParser.o SEMode.o main.o CHMiniMax.o BoardCell.o CHColorWindow.o CHDifficultyWindow.o CHLoadWindow.o CHModeWindow.o CHPiece.o CHStartWindow.o SEParser.o CHMainAux.o SPWindow.o Widget.o saveAndLoad.o
EXEC = chessprog
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/Homebrew/Library -Wl,-rpath,/usr/local/Homebrew/Library -Wl, -lSDL2 -lSDL2main
VPATH = graphics

all:	$(EXEC)
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
SPArrayList.o: SPArrayList.h SPArrayList.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SEMode.o: SEMode.c SEMode.h CHGame.h SEParser.h SPBufferset.h saveAndLoad.h graphics/CHStartWindow.h graphics/CHModeWindow.h graphics/SPSimpleMainWindow.h graphics/SimpleButton.h graphics/CHDifficultyWindow.h graphics/CHColorWindow.h graphics/CHLoadWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHMoves.o: CHMoves.c CHMoves.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHGame.o: CHGame.c CHGame.h SPArrayList.h CHMoves.h SPBufferset.h graphics/SPSimpleMainWindow.h graphics/gameBoard.h CHMiniMax.h graphics/Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
main.o: main.c CHMainAux.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHMiniMax.o: CHMiniMax.c CHMiniMax.h CHMoves.h CHGame.h SPArrayList.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SEParser.o: SEParser.c SEParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHParser.o: CHParser.c CHParser.h SEParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHMainAux.o: CHMainAux.c CHMainAux.h CHGame.h CHParser.h SEMode.h CHMiniMax.h gameBoard.h saveAndLoad.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
saveAndLoad.o: saveAndLoad.c saveAndLoad.h CHGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
BoardCell.o: graphics/BoardCell.c graphics/BoardCell.h graphics/SPCommon.h CHMoves.h graphics/SimpleButton.h graphics/Widget.h graphics/CHPiece.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
CHColorWindow.o: graphics/CHColorWindow.c graphics/CHColorWindow.h graphics/SPSimpleMainWindow.h graphics/SimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
CHDifficultyWindow.o: graphics/CHDifficultyWindow.c graphics/CHDifficultyWindow.h graphics/SPSimpleMainWindow.h graphics/SimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
CHLoadWindow.o: graphics/CHLoadWindow.c graphics/CHLoadWindow.h graphics/SimpleButton.h graphics/SPSimpleMainWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
CHModeWindow.o: graphics/CHModeWindow.c graphics/CHModeWindow.h graphics/SPSimpleMainWindow.h graphics/SimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
CHPiece.o: graphics/CHPiece.c graphics/CHPiece.h graphics/SPCommon.h CHMoves.h graphics/Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
CHStartWindow.o: graphics/CHStartWindow.c graphics/CHStartWindow.h graphics/SPSimpleMainWindow.h graphics/SimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
SPSimpleMainWindow.o: graphics/SPSimpleMainWindow.c graphics/SPSimpleMainWindow.h graphics/SimpleButton.h graphics/gameBoard.h CHGame.h graphics/SPWindow.h graphics/Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
SPWindow.o: graphics/SPWindow.c graphics/SPWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
SPCommon.o: graphics/SPCommon.c graphics/SPCommon.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
gameBoard.o: graphics/gameBoard.c graphics/gameBoard.h graphics/SPCommon.h graphics/SimpleButton.h CHGame.h graphics/Widget.h graphics/BoardCell.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
Widget.o: Widget.c Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
SimpleButton.o: graphics/SimpleButton.c SimpleButton.h graphics/SPCommon.h Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)