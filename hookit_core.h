#ifndef HOOKIT_CORE_H
#define HOOKIT_CORE_H
#include <sys/mman.h>
#include "hookit.h"

#define PAGESIZE 4096
#define MAXHOOKS 1024
#define SCAN_TIME 5
#define WP_OFF (PROT_READ | PROT_EXEC | PROT_WRITE)
#define WP_ON  (PROT_EXEC | PROT_READ)
#define PAGE_START(addr) (~(PAGESIZE - 1) & addr)

static hookit_entry entry[MAXHOOKS] = {0};
static u32 size = 0;

i8 hookit_wp_off(hookit_addr);
i8 hookit_wp_on(hookit_addr);

#endif
