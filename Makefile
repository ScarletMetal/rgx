CC=clang
CFLAGS= -I. -std=c89 -I'./include/' -largp -g

src := $(shell find ./src -type f -name *.c)
obj := $(patsubst ./src/%, ./build/%, $(src:.c=.o))

rgx: $(obj)
	$(CC) -o $@ $^ $(CFLAGS)

./build/%.o: ./src/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean

clean: 
	rm $(obj)

MKDIR_P ?= mkdir -p
