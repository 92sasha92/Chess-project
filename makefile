CC = gcc
OBJS = CHMoves.o CHGame.o SimpleButton.o SPSimpleMainWindow.o SPCommon.o gameBoard.o SPArrayList.o CHParser.o SEMode.o main.o CHMiniMax.o BoardCell.o CHColorWindow.o CHDifficultyWindow.o CHLoadWindow.o CHModeWindow.o CHPiece.o CHStartWindow.o SEParser.o CHMainAux.o SPWindow.o Widget.o saveAndLoad.o
EXEC = chessprog
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/Homebrew/Library -Wl,-rpath,/usr/local/Homebrew/Library -Wl, -lSDL2 -lSDL2main

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
SPArrayList.o: SPArrayList.h SPArrayList.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SEMode.o: SEMode.c SEMode.h CHGame.h SEParser.h SPBufferset.h saveAndLoad.h CHStartWindow.h CHModeWindow.h SPSimpleMainWindow.h SimpleButton.h CHDifficultyWindow.h CHColorWindow.h CHLoadWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHMoves.o: CHMoves.c CHMoves.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHGame.o: CHGame.c CHGame.h SPArrayList.h CHMoves.h SPBufferset.h SPSimpleMainWindow.h gameBoard.h CHMiniMax.h Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
main.o: main.c CHMainAux.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHMiniMax.o: CHMiniMax.c CHMiniMax.h CHMoves.h CHGame.h SPArrayList.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
BoardCell.o: BoardCell.c BoardCell.h SPCommon.h CHMoves.h SimpleButton.h Widget.h CHPiece.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHColorWindow.o: CHColorWindow.c CHColorWindow.h SPSimpleMainWindow.h SimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHDifficultyWindow.o: CHDifficultyWindow.c CHDifficultyWindow.h SPSimpleMainWindow.h SimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHLoadWindow.o: CHLoadWindow.c CHLoadWindow.h SimpleButton.h SPSimpleMainWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHModeWindow.o: CHModeWindow.c CHModeWindow.h SPSimpleMainWindow.h SimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c 
CHPiece.o: CHPiece.c CHPiece.h SPCommon.h CHMoves.h Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHStartWindow.o: CHStartWindow.c CHStartWindow.h SPSimpleMainWindow.h SimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SEParser.o: SEParser.c SEParser.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPCommon.o: SPCommon.c SPCommon.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHMainAux.o: CHMainAux.c CHMainAux.h CHGame.h CHParser.h SEMode.h CHMiniMax.h gameBoard.h saveAndLoad.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPSimpleMainWindow.o: SPSimpleMainWindow.c SPSimpleMainWindow.h SimpleButton.h gameBoard.h CHGame.h SPWindow.h Widget.h	
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPWindow.o: SPWindow.c SPWindow.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SimpleButton.o: SimpleButton.c SimpleButton.h SPCommon.h Widget.h	
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Widget.o: Widget.c Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
gameBoard.o: gameBoard.c gameBoard.h SPCommon.h SimpleButton.h CHGame.h Widget.h BoardCell.h	
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
saveAndLoad.o: saveAndLoad.c saveAndLoad.h CHGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHParser.o: CHParser.c CHParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)