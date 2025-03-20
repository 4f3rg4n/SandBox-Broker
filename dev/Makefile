CC = gcc
SOFLAGS = -shared -fPIC
CFLAGS = -I.

hook.so: hook.c hook.h
	${CC} ${SOFLAGS} ${CFLAGS} -o $@ $< -ldl

clean:
	rm -f hook.so
