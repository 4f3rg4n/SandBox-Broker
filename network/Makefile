CC = gcc
SOFLAGS = -shared -fPIC
CFLAGS = -I.

network_hook.so: network_hook.c network_hook.h
	${CC} ${SOFLAGS} ${CFLAGS} -o $@ $< -ldl

clean:
	rm -f network_hook.so
