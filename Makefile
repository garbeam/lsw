# lsw - list window names
#   (C)opyright MMVI Anselm R. Garbe

include config.mk

SRC = lsw.c
OBJ = ${SRC:.c=.o}

all: options lsw
	@echo finished

options:
	@echo lsw build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

lsw: ${OBJ}
	@echo LD $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}
	@strip $@

clean:
	@echo cleaning
	@rm -f lsw ${OBJ} lsw-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p lsw-${VERSION}
	@cp -R LICENSE Makefile README config.mk ${SRC} lsw-${VERSION}
	@tar -cf lsw-${VERSION}.tar lsw-${VERSION}
	@gzip lsw-${VERSION}.tar
	@rm -rf lsw-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f lsw ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/lsw

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/lsw

.PHONY: all options clean dist install uninstall
