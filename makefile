CC = gcc
OBJS = CHMoves.o CHGame.o SPArrayList.o SEParser.o CHParser.o SEMode.o main.o CHMiniMax.o 
EXEC = chessprog
GAME_TEST_OBJS = SPArrayList.o CHGame.o SPFIARGameUnitTest.o
ARRAY_LIST_TEST_OBJS = SPArrayListUnitTest.o SPArrayList.o
PARSER_TEST_OBJS = SEParser.o SPFIARParserUnitTest.o
MINMAXNode_TEST_OBJ = SPMiniMaxNodeUnitTest.o CHGame.o SPArrayList.o
MINMAX_TEST_OBJ = SPMiniMaxNode.o SPMiniMaxUnitTest.o CHGame.o SPArrayList.o
MAINAUX_TEST_OBJS = SPMainAux.o SPArrayList.o CHGame.o
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
UNIT_TESTS = SPFIARGameUnitTest SPArrayListUnitTest SEParser SPMiniMaxUnitTest\
SPMiniMaxNodeUnitTest SPMainAuxUnitTest

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
SPFIARGameUnitTest: $(GAME_TEST_OBJS)
	$(CC) $(GAME_TEST_OBJS) -o $@
SPArrayListUnitTest: $(ARRAY_LIST_TEST_OBJS)
	$(CC) $(ARRAY_LIST_TEST_OBJS) -o $@
SPFIARParserUnitTest: $(PARSER_TEST_OBJS)
	$(CC) $(PARSER_TEST_OBJS) -o $@
SPMiniMaxUnitTest: $(MINMAX_TEST_OBJ)
	$(CC) $(MINMAXNode_TEST_OBJ) -o $@
SPMiniMaxNodeUnitTest: $(MINMAXNode_TEST_OBJ)
	$(CC) $(MINMAXNode_TEST_OBJ) -o $@
SPMainAuxUnitTest: $(MAINAUX_TEST_OBJS)
	$(CC) $(MAINAUX_TEST_OBJS) -o $@
SPFIARGameUnitTest.o: SPFIARGameUnitTest.c unit_test_util.h SPArrayList.h CHGame.h
	$(CC) $(COMP_FLAG) -c $*.c
SPArrayListUnitTest.o: SPArrayListUnitTest.c SPArrayList.h unit_test_util.h
	$(CC) $(COMP_FLAG) -c $*.c
SPFIARParserUnitTest.o: SPFIARParserUnitTest.c SEParser.h unit_test_util.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMiniMaxUnitTest.o: SPMiniMaxUnitTest.c CHGame.h unit_test_util.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMiniMaxNodeUnitTest.o: SPMiniMaxNodeUnitTest.c unit_test_util.h CHGame.h
	$(CC) $(COMP_FLAG) -c $*.c
SPArrayList.o: SPArrayList.h SPArrayList.c
	$(CC) $(COMP_FLAG) -c $*.c
SEMode.o: SEMode.c SEMode.h CHGame.h SEParser.h SPBufferset.h
	$(CC) $(COMP_FLAG) -c $*.c
CHMoves.o: CHMoves.c CHMoves.h
	$(CC) $(COMP_FLAG) -c $*.c
CHGame.o: CHGame.c CHGame.h SPArrayList.h CHMoves.h
	$(CC) $(COMP_FLAG) -c $*.c
SEParser.o: SEParser.c SEParser.h
	$(CC) $(COMP_FLAG) -c $*.c
CHParser.o: CHParser.c CHParser.h SEParser.h
	$(CC) $(COMP_FLAG) -c $*.c	
main.o: main.c CHGame.h CHParser.h SEMode.h CHMiniMax.h
	$(CC) $(COMP_FLAG) -c $*.c
CHMiniMax.o: CHMiniMax.c CHMiniMax.h CHMoves.h CHGame.h SPArrayList.h
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
