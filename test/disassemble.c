#include <stdio.h>
#include "../capstone/capstone.h"

int add(int a, int b) {
    return a + b;
}

int main(int argc, char **argv) {
    csh handle;
    cs_insn * ins;
    size_t count;
    if(cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK) {
        return -1;
    }
    int base = 14;
    int most = 50;
    for(int i = base; i < most; i++) {
        count = cs_disasm(handle, (unsigned char*)add, i, 0x1000, 0, &ins);
        if(count > 0) {
            size_t j;
            int t_size = 0;
            for(j = 0; j < count; j++) {
                t_size += ins[j].size;
            }
            if(t_size >= base) {
                printf("%d\n", t_size);
                break;
            }
            cs_free(ins, count);
        } else {
            printf("Error\n");
        }
    }
    cs_close(&handle);
    return 0;
}