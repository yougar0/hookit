#include <stdio.h>
#include <sys/mman.h>
#include "../capstone/capstone.h"
#include <stdlib.h>
#include <string.h>
#include <error.h>
typedef unsigned char u8;
#define PAGESIZE 4096
#define PAGE_START(page) (~(PAGESIZE - 1) & page)

int (*old_add)(int, int);

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    *(&a) = a + 2;
    return old_add(a, b);
}

int main(int argc, char** argv) {
    csh handle;
    cs_insn * ins;
    size_t count;
    int base = 14;
    int most = 50;
    int size = 0;
    size_t j = 0;
    if(cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK) {
        return -1;
    }

    u8* addr = add;
    for(int i = base; i < most; i++) {
        count = cs_disasm(handle, addr, i, 0, 0, &ins);
        if(count > 0) {
            for(j = 0; j < count; j++) {
                size += ins[j].size;
            }
            if(size >= base) {
                break;
            }
            cs_free(ins, count);
        } else {
            printf("Error\n");
        }
    }
    cs_close(&handle);
    u8* trampline = (u8*)mmap(NULL, size + 28, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if(trampline == MAP_FAILED) {
        perror("mmap error");
        return -1;
    }
    trampline[0] = 0xFF;
    trampline[1] = 0x25;
    *(unsigned int*)&trampline[2] = 0x00000000;
    *(unsigned long*)&trampline[6] = (unsigned long)sub;
    memcpy(trampline + 14, addr, size);
    base = 14 + size;
    trampline[base] = 0xFF;
    trampline[base + 1] = 0x25;
    *(unsigned int*)&trampline[base + 2] = 0x00000000;
    printf("ok\n");
    *(unsigned long*)&trampline[base + 6] = (unsigned long)addr + size;
    mprotect(PAGE_START((unsigned long)add), PAGESIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
    addr[0] = 0xFF;
    addr[1] = 0x25;
    *(unsigned int*)&addr[2] = 0x00000000;
    *(unsigned long*)&addr[6] = (unsigned long)trampline;
    mprotect(PAGE_START((unsigned long)add), PAGESIZE, PROT_READ | PROT_EXEC);
    int a = 2, b =2;
    old_add = &trampline[14];
    printf("%d + %d = %d\n", a, b, add(a, b));
    munmap(trampline, size + 28);
    return 0;
}