CC = gcc

sandbox: files_hook.so network_hook.so
	${CC} -o sandbox sandbox.c

files_hook.so:
	make -C files

network_hook.so:
	make -C network

clean:
	make clean -C files
	make clean -C network
	rm -f sandbox
