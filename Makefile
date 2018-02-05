shared:
	gcc -fPIC -shared -o libhookit.so hookit_core.c
push:
	$(shell cp hookit.h libhookit.so test/)
