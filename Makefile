CFLAGS=-Wall -Wextra

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst src/%.c,src/%.o,$(SOURCES))

TARGET=bin/CubeTree

all: $(TARGET)

$(TARGET): bin/ $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o:%.c
	$(CC) -c -o $@ $<

bin/:
	@mkdir -p bin

clean:
	rm -rf bin
	rm -f src/*.o
	rm -f src/**/*.o
