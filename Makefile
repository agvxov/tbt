OBJ:=obj/
SRC:=source/

MAIN:=source/main.cpp
SRCF:=source/io.cpp source/format.cpp source/globals.cpp source/parse.cpp source/errno.cpp
HF:=${SRCF:.cpp=.h} source/help.h source/table.h
OF:=$(subst source/,obj/,${SRCF:.cpp=.o})
GITF=${MAIN} ${SRCF} ${HF} Makefile docs/*
VERSION:=$(shell git tag | tail -1 | cut -c2-)

OUTPUT=main.out

CXX:=g++
CXXFLAGS:=-g -std=c++17
LD_FLAGS:=

main:
	${CXX} ${MAIN} ${SRCF} -o ${OUTPUT} ${CXXFLAGS}

production:
	${CXX} ${MAIN} ${SRCF} -o tbt ${CXXFLAGS} -O3

clean:
	-rm -v ${OF}
	-rm ${OUTPUT}
	-rm tbt

debug:
	gdb -q ${OUTPUT}

git:
	git add ${GITF}
	git commit ${GITF}

tarball:
	tarball_from_project

dist: tarball
	mv tbt-${VERSION}.tar.gz /var/cache/distfiles/

install: production
	mkdir -p ${DESTDIR}/usr/bin
	mv tbt ${DESTDIR}/usr/bin/
