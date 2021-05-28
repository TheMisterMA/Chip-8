MODULE_UNDER_TEST=Chip8.cpp
UNIT_TEST=UnitTests/MainTest.cpp
SOURCES_PATH=-isystem ./

TEST_FILES=UnitTests/BC_test.ch8
TEST_CONFIG=  10 1 ${TEST_FILES}

# SDL options
CC_SDL=-lSDL2 `sdl2-config --cflags --libs`

cmp:
	g++ ${MODULE_UNDER_TEST} ${UNIT_TEST} ${SOURCES_PATH}

run:
	g++ ${MODULE_UNDER_TEST} PlatformInterface.cpp main.cpp ${SOURCES_PATH} ${CC_SDL}
	./a.out ${TEST_CONFIG}
