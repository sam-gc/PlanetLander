CORE_SOURCES=$(wildcard src/core/*.c)
TOOLS_SOURCES=$(wildcard src/tools/*.c)

CORE_OBJ_FILES=$(addprefix obj/core/,$(notdir $(CORE_SOURCES:.c=.o)))
TOOLS_OBJ_FILES=$(addprefix obj/tools/,$(notdir $(TOOLS_SOURCES:.c=.o)))

CFLAGS=-g -Isrc -std=c99 -Wall `sdl2-config --cflags`
LDFLAGS=-lm -lGL -lGLEW `sdl2-config --libs`
CC=gcc
MKDIR=mkdir -p
SHADER_GEN=python2 src/shaders/embed.py

all: PlanetLander

PlanetLander: shaders core

core: $(CORE_OBJ_FILES) $(TOOLS_OBJ_FILES)
	$(CC) $(CFLAGS) -o PlanetLander $^ $(LDFLAGS)

clean:
	rm -f PlanetLander
	rm -rf obj
	rm -f src/shaders/vertex__gen.h
	rm -f src/shaders/fragment__gen.h

obj/core/%.o: src/core/%.c
	$(MKDIR) obj/core/
	$(CC) $(CFLAGS) -c -o $@ $<

obj/tools/%.o: src/tools/%.c
	$(MKDIR) obj/tools/
	$(CC) $(CFLAGS) -c -o $@ $<

shaders: src/shaders/vertex.glsl src/shaders/fragment.glsl
	$(SHADER_GEN) src/shaders/vertex.glsl
	$(SHADER_GEN) src/shaders/fragment.glsl
