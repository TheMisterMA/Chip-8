MODULE_UNDER_TEST=Chip8.cpp
UNIT_TEST=UnitTests/MainTest.cpp
SOURCES_PATH=-isystem ./

cmp:
	g++ ${MODULE_UNDER_TEST} ${UNIT_TEST} ${SOURCES_PATH}

run: cmp
	./a.out
