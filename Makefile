CFLAGS=-Wall -Wextra

SOURCES=$(shell find src/ -type f -name "*.c")
OBJECTS=$(patsubst src/%.c,obj/%.o,$(SOURCES))

TARGET=bin/CubeTree

all: $(TARGET)

$(TARGET): bin/ $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

obj/%.o:src/%.c
	@mkdir -p "$(@D)"
	$(CC) -c -o $@ $<

bin/:
	@mkdir -p bin

clean:
	@rm -rf bin
	@rm -rf obj
