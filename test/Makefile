test:
	gcc -o test_$(n) test_$(n).c -ldl

clean:
	rm test_$(n)

dis:
	gcc -o disasm disassemble.c -L$(shell pwd)/../ -lcapstone -Wl,-R $(shell pwd)/../
