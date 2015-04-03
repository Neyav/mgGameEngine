# Compiler.. In this case it's langc's c++ compiler that ships standard
# with FreeBSD 10
CC = c++
STRIP = strip
RM = rm

CFLAGS = -O2

mgConsoleTest: ConsoleMazeGenerator.o mgLineSegment.o mgMapDataHandler.o mgMapElement.o mgMapObject.o mgMapObjectPlayer.o mgPathSolutionGenerator.o mgRayTracer.o mgStressTest.o mgVectorPoint.o mgVisibilityMap.o mgRandomMazeGenerator.o
	$(CC) -o mgConsoleTest ConsoleMazeGenerator.o mgLineSegment.o mgMapDataHandler.o mgMapElement.o mgMapObject.o mgMapObjectPlayer.o mgPathSolutionGenerator.o mgRayTracer.o mgStressTest.o mgVectorPoint.o mgVisibilityMap.o mgRandomMazeGenerator.o
	$(STRIP) --strip-all mgConsoleTest

NCursesExplorer: NCursesExplorer.o mgLineSegment.o mgMapDataHandler.o mgMapElement.o mgVectorPoint.o mgRayTracer.o mgRandomMazeGenerator.o 
	$(CC) -lncurses -o NCursesExplorer NCursesExplorer.o mgLineSegment.o mgMapDataHandler.o mgMapElement.o mgVectorPoint.o mgRandomMazeGenerator.o mgRayTracer.o
	$(STRIP) --strip-all NCursesExplorer

NCursesExplorer.o: Source/NCursesExplorer.cpp
	$(CC) $(CFLAGS) -o NCursesExplorer.o -c Source/NCursesExplorer.cpp

ConsoleMazeGenerator.o: Source/ConsoleMazeGenerator.cpp
	$(CC) $(CFLAGS) -o ConsoleMazeGenerator.o -c Source/ConsoleMazeGenerator.cpp

mgLineSegment.o: Source/mgLineSegment.cpp Source/mgLineSegment.h
	$(CC) $(CFLAGS) -o mgLineSegment.o -c Source/mgLineSegment.cpp

mgMapDataHandler.o: Source/mgMapDataHandler.cpp Source/mgMapDataHandler.h
	$(CC) $(CFLAGS) -o mgMapDataHandler.o -c Source/mgMapDataHandler.cpp

mgMapElement.o: Source/mgMapElement.cpp Source/mgMapElement.h
	$(CC) $(CFLAGS) -o mgMapElement.o -c Source/mgMapElement.cpp

mgMapObject.o: Source/mgMapObject.cpp Source/mgMapObject.h
	$(CC) $(CFLAGS) -o mgMapObject.o -c Source/mgMapObject.cpp

mgMapObjectPlayer.o: Source/mgMapObjectPlayer.cpp Source/mgMapObjectPlayer.h
	$(CC) $(CFLAGS) -o mgMapObjectPlayer.o -c Source/mgMapObjectPlayer.cpp

mgPathSolutionGenerator.o: Source/mgPathSolutionGenerator.cpp Source/mgPathSolutionGenerator.h
	$(CC) $(CFLAGS) -o mgPathSolutionGenerator.o -c Source/mgPathSolutionGenerator.cpp

mgRandomMazeGenerator.o: Source/mgRandomMazeGenerator.cpp Source/mgRandomMazeGenerator.h
	$(CC) $(CFLAGS) -o mgRandomMazeGenerator.o -c Source/mgRandomMazeGenerator.cpp

mgStressTest.o: Source/mgStressTest.cpp Source/mgStressTest.h
	$(CC) $(CFLAGS) -o mgStressTest.o -c Source/mgStressTest.cpp

mgVectorPoint.o: Source/mgVectorPoint.cpp Source/mgVectorPoint.h
	$(CC) $(CFLAGS) -o mgVectorPoint.o -c Source/mgVectorPoint.cpp

mgVisibilityMap.o: Source/mgVisibilityMap.cpp Source/mgVisibilityMap.h
	$(CC) $(CFLAGS) -o mgVisibilityMap.o -c Source/mgVisibilityMap.cpp

mgRayTracer.o: Source/mgRayTracer.cpp Source/mgRayTracer.h
	$(CC) $(CFLAGS) -o mgRayTracer.o -c Source/mgRayTracer.cpp

clean:
	$(RM) *.o

clean-all:
	$(RM) mgConsoleTest *.o
