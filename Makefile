OBJ:=obj/
SRC:=source/

MAIN:=source/main.cpp
SRCF:=source/io.cpp source/format.cpp source/globals.cpp source/parse.cpp source/errno.cpp
HF:=${SRCF:.cpp=.h} source/help.h
OF:=$(subst source/,obj/,${SRCF:.cpp=.o})
GITF=${MAIN} ${SRCF} ${HF} Makefile docs/*

OUTPUT=main.out

CXX:=g++
CXXFLAGS:=-g -std=c++17
LD_FLAGS:=

main:
	${CXX} ${MAIN} ${SRCF} -o ${OUTPUT} ${CXXFLAGS}

clean:
	rm -v ${OF}

debug:
	gdb -q ${OUTPUT}

git:
	git add ${GITF}
	git commit ${GITF}
