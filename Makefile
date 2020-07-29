CC=clang
CFLAGS= -I. -std=c89 -I'./include/' -largp

src=$(wildcard ./src/*.c)
obj=$(src:.c=.o)

rgx: $(obj)
	$(CC) -o $@ $^ $(CFLAGS)
	rm $(obj)

clean: 
	rm $(obj)
