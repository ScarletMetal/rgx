CC=cc
CFLAGS= -I. -std=c89 -I'./include/' -g

src := $(shell find ./src -type f -name *.c)
obj := $(patsubst ./src/%, ./build/src/%, $(src:.c=.o))

test_src := $(shell find ./test -type f -name *.c)
test_obj := $(patsubst ./test/%, ./build/test/%, $(test_src:.c=.o))

lib: $(obj)
	$(CC) -shared -o rgx.so $^ $(CFLAGS)

test: $(obj) $(test_obj)
	$(CC) -o rgx $^ $(CFLAGS)

all:
	make exec
	make lib

./build/src/%.o: ./src/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

./build/test/%.o: ./test/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean test

test:
	echo $(test_src)
	echo $(test_obj)

clean: 
	rm $(obj)

MKDIR_P ?= mkdir -p
