PORT:=17629
LIBS:=libnm libelogind
CC:=gcc -g -Wall -Wextra -std=gnu11
PC_C:=$(shell pkg-config --cflags $(LIBS))
PC_L:=$(shell pkg-config --libs   $(LIBS))
BIN:=coldspot

default: run

run: $(BIN)
	[ aarch64 = "$(shell uname -m)" ]
	@echo 'run'
	@echo 'foo' | busybox nc -w0 127.0.0.1 $(PORT)

$(BIN): main.c.o hier.c.o
	:; $(CC) -o $@ $^ $(PC_L)

%.c.o: %.c
	:; $(CC) -c $(PC_C) -o $@ $^

clean:
	@rm -fv $(BIN)
	@rm -fv *.c.o
