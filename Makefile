OBJ:=obj/
SRC:=source/

SRCF:=source/main.cpp source/io.cpp source/format.cpp source/globals.cpp source/parse.cpp
HF:=${SRCF:.cpp=.h}
OF:=${SRCF:.cpp=.o}
GITF=${SRCF} Makefile docs/*

main:
	gcc ${SRCF} -o main.out

git:
	git add ${GITF}
	git commit ${GITF}
