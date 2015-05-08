CXX = g++
CXXFLAGSP = -L/usr/X11R6/lib
CXXFLAGS = -Wall -MMD -g -lX11 -lpthread
EXEC = quadris
OBJECTS = main.o board.o block.o level.o qdisplay.o trie.o window.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGSP} ${OBJECTS} ${CXXFLAGS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
