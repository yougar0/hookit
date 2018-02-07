#include "hookit_func.h"
#include "capstone/capstone.h"
#include <stdlib.h>

struct hookit_func_type {
    void* addr;
    struct hookit_func_type* next;
    u8* trampline;
};

struct hookit_func_type* hooked_func_list_head = NULL;
struct hookit_func_type* hooked_func_list_tail = NULL;

i8 hookit_func(void* origin_func, void* replace_func) {
    if(hookit_func_is_hooked(origin_func)) {
        return HOOKIT_FUNC_IS_HOOKED;
    }
    struct hookit_func_type* data = hookit_func_add_func(origin_func);
    return hookit_func_hook_func(data, replace_func);
}

u8 hookit_func_is_hooked(void* func) {
    if(hookit_func_find_hook(func) || !hookit_func_scan_func_valid(func)) {
        return 1;
    }
    return 0;
}

u8 hookit_func_find_hook(void* func) {
    if(hooked_func_list_head != NULL) {
        struct hookit_func_type* temp;
        temp = hooked_func_list_head;
        while(temp != NULL) {
            if(temp->addr == func) {
                return 1;
            }
            temp = temp->next;
        }
        
    }
    return 0;
}

u8 hooked_func_scan_func_valid(void* func) {
    return *(u8*)func == HOOKIT_FUNC_PUSH;
}

struct hookit_func_type *hookit_func_add_func(void* func) {
    struct hookit_func_type* new_func = (struct hookit_func_type*)malloc(sizeof(struct hookit_func_type));
    new_func->addr = func;
    hooked_func_list_tail->next = new_func;
    hooked_func_list_tail = new_func;
    return new_func;    
}

i8 hookit_func_hook_func(struct hookit_func_type* data, void* func) {
    
}