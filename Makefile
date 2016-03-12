CFLAGS=-std=gnu99 -Wall -Wextra -O3 -lpthread

SOURCES=$(shell find src/ -type f -name "*.c")
OBJECTS=$(patsubst src/%.c,obj/%.o,$(SOURCES))

TARGET=bin/a.out

.SECONDARY:

all: $(TARGET)

$(TARGET): bin/ $(OBJECTS)
	@$(CC) -s $(OBJECTS) -o $@ $(CFLAGS)

obj/%.o: src/%.c
	@mkdir -p "$(@D)"
	$(CC) -c -I include/ -o $@ $< $(CFLAGS)

bin/:
	@mkdir -p bin

clean:
	@rm -rf bin
	@rm -rf obj
