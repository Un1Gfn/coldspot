PORT:=17629

default: run

run: coldspot
	@echo 'run'
	@echo 'foo' | busybox nc -w0 127.0.0.1 $(PORT)

coldspot: main.c
	./build.sh
