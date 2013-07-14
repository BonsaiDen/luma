FILES:=src/*.c
DEPS:=deps/lib/*.a
LIBS:=-lX11 -lXxf86vm -lGL -lXrandr -lXi -lrt -lasound

CFLAGS=-o main -Wall -Wextra -Wno-unused 

build: $(FILES) $(DEPS)
	xxd -i src/boot.lua > include/boot.lua.h
	gcc $(CFLAGS) $(FILES) $(DEPS) $(LIBS)
	strip main

debug: $(FILES)
	gcc $(CFLAGS) $(FILES) $(DEPS) $(LIBS) -ggdb

