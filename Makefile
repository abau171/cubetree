CFLAGS=-std=gnu99 -Wall -Wextra

SOLVER_SOURCES=$(shell find src/ -mindepth 1 -maxdepth 1 -type f -name "*.c") $(shell find src/solver/ -type f -name "*.c")
SOLVER_OBJECTS=$(patsubst src/%.c,obj/%.o,$(SOLVER_SOURCES))

PUZZLE_MOD_ROOTS=$(shell find src/puzzles/ -mindepth 1 -maxdepth 1 -type d)
PUZZLE_MOD_OBJECTS=$(patsubst src/puzzles/%,obj/puzzles/%.o,$(PUZZLE_MOD_ROOTS))
ALL_PUZZLE_SOURCES=$(shell find src/puzzles/ -type f -name "*.c")
ALL_PUZZLE_OBJECTS=$(patsubst src/puzzles/%.c,obj/puzzles/%.o,$(ALL_PUZZLE_SOURCES))

TARGET=bin/CubeTree

.SECONDARY:

all: $(TARGET)

$(TARGET): bin/ $(SOLVER_OBJECTS) $(PUZZLE_MOD_OBJECTS)
	@echo ""
	@echo "Linking Solver and Puzzle Modules..."
	@$(CC) -s $(SOLVER_OBJECTS) $(PUZZLE_MOD_OBJECTS) -o $@ $(CFLAGS)
	@echo ""

obj/puzzles/%.o: src/puzzles/% $(ALL_PUZZLE_OBJECTS)
	$(eval PUZZLE_MOD_ID := $(patsubst obj/puzzles/%.o,%,$@))
	$(eval PUZZLE_MOD_OBJ_ROOT := $(patsubst obj/puzzles/%.o,obj/puzzles/%/,$@))
	$(eval PUZZLE_OBJECTS := $(shell find $(PUZZLE_MOD_OBJ_ROOT) -type f -name "*.o"))
	@echo ""
	@echo "Linking Puzzle Module: '$(PUZZLE_MOD_ID)'"
	@$(LD) -r $(PUZZLE_OBJECTS) -o $@
	@objcopy -w --localize-symbol=* $@

obj/%.o: src/%.c
	@mkdir -p "$(@D)"
	$(CC) -c -o $@ $< $(CFLAGS)

bin/:
	@mkdir -p bin

clean:
	@rm -rf bin
	@rm -rf obj
