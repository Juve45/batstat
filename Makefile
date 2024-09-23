CC=g++

CCFLAGS=-std=c++11
LDFLAGS=-lncurses -pthread

BIN=batstat
CONFIG=/etc/

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

all: build

clean:
	@rm $(BIN)

build: main.cpp
	$(CC) -o $(BIN) $(CCFLAGS) $< $(LDFLAGS)

copy: config/batstat.json
	cp -f $< $(CONFIG)

install: copy
	install -m 775 $(BIN) $(DESTDIR)$(PREFIX)/bin
