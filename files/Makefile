CC = gcc
SOFLAGS = -shared -fPIC
CFLAGS = -I.

files_hook.so: files_hook.c files_hook.h
	${CC} ${SOFLAGS} ${CFLAGS} -o $@ $< -ldl

clean:
	rm -f files_hook.so
