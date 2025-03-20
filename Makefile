CC = gcc

sandbox: files_hook.so network_hook.so
	${CC} -o sandbox sandbox.c

files_hook.so:
	make -C dev/files

network_hook.so:
	make -C dev/network

clean:
	make clean -C dev/files
	make clean -C dev/network
	rm -f sandbox
