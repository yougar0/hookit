#include <stdio.h>
#include <sys/mman.h>
typedef unsigned char u8;
#define PAGESIZE 4096
#define PAGE_START(page) (~(PAGESIZE - 1) & page)

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

int main(int argc, char** argv) {
    mprotect(PAGE_START((unsigned long)add), PAGESIZE, PROT_EXEC | PROT_READ | PROT_WRITE);
    u8* addr = add;
    addr[0] = 0xFF;
    addr[1] = 0x25;
    *(unsigned int*)&addr[2] = 0x00000000;
    *(unsigned long*)&addr[6] = sub;
    int a = 2, b =2;
    printf("%d + %d = %d\n", a, b, add(a, b));
    mprotect(PAGE_START((unsigned long)add), PAGESIZE, PROT_READ | PROT_WRITE);
    return 0;
}
