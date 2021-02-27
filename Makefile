UNIT_TEST=tests/MainTest.cpp
SOURCES_PATH=-isystem ./

cmp:
	g++ ${UNIT_TEST} ${SOURCES_PATH}
