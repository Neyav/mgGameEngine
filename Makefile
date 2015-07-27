# Build parameters
CC = c++
STRIP = strip
RM = rm

CFLAGS = -std=c++11 -O2
LINKERFLAGS = -lm -lstdc++

# The default build target. This is the console test engine.
mgConsoleTest: ConsoleMazeGenerator.o mgLineSegment.o mgMapDataHandler.o mgMapElement.o mgMapObject.o mgPathSolutionGenerator.o mgRayTracer.o mgCollisionDetection.o mgStressTest.o mgVectorPoint.o mgVisibilityMap.o mgRandomMazeGenerator.o
	$(CC) -o mgConsoleTest ConsoleMazeGenerator.o mgLineSegment.o mgMapDataHandler.o mgMapElement.o mgMapObject.o mgPathSolutionGenerator.o mgRayTracer.o mgCollisionDetection.o mgStressTest.o mgVectorPoint.o mgVisibilityMap.o mgRandomMazeGenerator.o $(LINKERFLAGS)
	$(STRIP) --strip-all mgConsoleTest

# NCursesExplorer build target. This is a NCurses based first person renderer for debug purposes
NCursesExplorer: NCursesExplorer.o mgLineSegment.o mgMapDataHandler.o mgMapElement.o mgVectorPoint.o mgRayTracer.o mgRandomMazeGenerator.o mgCollisionDetection.o mgMapObject.o
	$(CC) -o NCursesExplorer NCursesExplorer.o mgLineSegment.o mgMapDataHandler.o mgMapElement.o mgVectorPoint.o mgRandomMazeGenerator.o mgRayTracer.o mgCollisionDetection.o mgMapObject.o -lncurses $(LINKERFLAGS)
	$(STRIP) --strip-all NCursesExplorer

# SDLengine build target. This is a SDL2 based renderer for the engine. Top down view.
SDLengine: SDLengine.o SEViewDisplay.o SETextureHandler.o SERenderHandler.o mgLineSegment.o mgMapDataHandler.o mgMapElement.o mgMapObject.o mgPathSolutionGenerator.o mgRayTracer.o mgCollisionDetection.o mgVectorPoint.o mgVisibilityMap.o mgRandomMazeGenerator.o
	$(CC) -o SDLengine SDLengine.o SETextureHandler.o SERenderHandler.o mgLineSegment.o mgMapDataHandler.o mgMapElement.o mgMapObject.o mgPathSolutionGenerator.o mgRayTracer.o mgCollisionDetection.o mgVectorPoint.o mgVisibilityMap.o mgRandomMazeGenerator.o -lSDL2 -lSDL2_image $(LINKERFLAGS)
	$(STRIP) --strip-all SDLengine

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

mgCollisionDetection.o: Source/mgCollisionDetection.cpp Source/mgCollisionDetection.h
	$(CC) $(CFLAGS) -o mgCollisionDetection.o -c Source/mgCollisionDetection.cpp

# [SDL ENGINE]
SETextureHandler.o: Source/SDLengine/SETextureHandler.cpp Source/SDLengine/SETextureHandler.h
	$(CC) $(CFLAGS) -o SETextureHandler.o -c Source/SDLengine/SETextureHandler.cpp

SERenderHandler.o: Source/SDLengine/SERenderHandler.cpp Source/SDLengine/SERenderHandler.h
	$(CC) $(CFLAGS) -o SERenderHandler.o -c Source/SDLengine/SERenderHandler.cpp

SEViewDisplay.o: Source/SDLengine/SEViewDisplay.cpp Source/SDLengine/SEViewDisplay.h
	$(CC) $(CFLAGS) -o SEViewDisplay.o -c Source/SDLengine/SEViewDisplay.cpp

SDLengine.o: Source/SDLengine/SDLengine.cpp
	$(CC) $(CFLAGS) -o SDLengine.o -c Source/SDLengine/SDLengine.cpp
#-[End SDL Engine]

clean:
	$(RM) *.o

clean-all:
	$(RM) *.o mgConsoleTest NCursesExplorer SDLengine
