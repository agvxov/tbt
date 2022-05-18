OBJ:=obj/
SRC:=source/

MAIN:=source/main.cpp
SRCF:=source/io.cpp source/format.cpp source/globals.cpp source/parse.cpp
HF:=${SRCF:.cpp=.h}
OF:=${SRCF:.cpp=.o}
GITF=${SRCF} ${HF} Makefile docs/*

main:
	gcc ${MAIN} ${SRCF} -o main.out

git:
	git add ${GITF}
	git commit ${GITF}
