# Makefile

CC=g++
CFLAGS=-std=c++11 -static-libgcc -static-libstdc++
BIN=../MazeGeneratorTest.exe
CFILES=ConsoleMazeGenerator.cpp mgLineSegment.cpp mgMapDataHandler.cpp mgMapElement.cpp mgMapObject.cpp mgMapObjectPlayer.cpp mgPathSolutionGenerator.cpp mgRandomMazeGenerator.cpp mgRayTracer.cpp mgStressTest.cpp mgVectorPoint.cpp mgVisibilityMap.cpp

$(BIN): Makefile $(CFILES)
	$(CC) $(CFLAGS) $(CFILES) -o $(BIN)

