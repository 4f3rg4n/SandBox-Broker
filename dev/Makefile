CC = gcc
SOflags = -ldl -shared -fPIC

hook.so: hook.c
	${CC} ${SOflags} -o $@ $< 

clean:
	rm -f hook.so