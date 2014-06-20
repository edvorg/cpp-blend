ANALYZER1= scan-build

INCLUDES=-Isrc/main

CC1= clang++
CC1_FLAGS= -std=c++1y -stdlib=libc++ -I. -O1 ${INCLUDES} -o a1.out
LL1_FLAGS= -lc++abi -lpthread

CC2= g++
CC2_FLAGS= -std=c++1y -I. -O1 ${INCLUDES} -o a2.out
LL2_FLAGS= -lc++abi -lpthread

CC3= em++
CC3_FLAGS= -std=c++1y -stdlib=libc++ -I. -O1 ${INCLUDES} -s DISABLE_EXCEPTION_CATCHING=0 -o a3.html
LL3_FLAGS=

all:
	echo analizing with ${CC1}
	${ANALYZER1} ${CC1} ${CC1_FLAGS} ${LL1_FLAGS} src/test/main.cpp

	echo compile with ${CC1}
	${CC1} ${CC1_FLAGS} ${LL1_FLAGS} src/test/main.cpp

	echo compile with ${CC2}
	${CC2} ${CC2_FLAGS} ${LL2_FLAGS} src/test/main.cpp

	echo compile with ${CC3}
	${CC3} ${CC3_FLAGS} ${LL3_FLAGS} src/test/main.cpp

check-syntax:
	${CC1} ${CC1_FLAGS} ${LL1_FLAGS} -fsyntax-only -fno-color-diagnostics ${CHK_SOURCES}

print-cflags:
	echo .
