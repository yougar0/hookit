#include "hookit.h"
#include "hookit_core.h"
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>

hookit_entry *hookit_init(void *parent, void *target, void *player) {
  if(size == MAXHOOKS) return NULL;

  hookit_entry *new_entry = &entry[size];
  new_entry->parent = (u8*)parent;
  new_entry->target = (u8*)target;
  new_entry->player = (u8*)player;
  return new_entry;
}

i8 hookit_wp_off(hookit_addr addr) {
  return mprotect((void*)PAGE_START(addr), PAGESIZE, WP_OFF);
}

i8 hookit_wp_on(hookit_addr addr) {
  return mprotect((void*)PAGE_START(addr), PAGESIZE, WP_ON);
}

i8 hookit(hookit_entry *entry) {
  if(entry == NULL) {
	printf("Hook engine: entry can't be NULL\n");
	return -1;
  }
  u8 buf[4];
  hookit_addr addr;
  u16 base = 0;
  printf("Hook engine: hook target func at: 0x%lx\n", (hookit_addr)entry->target);

  hookit_addr len = (hookit_addr)entry->parent % PAGESIZE;
  u8 *parent = entry->parent;

  for(u8 time = 0; time < SCAN_TIME; time++) {

    if(hookit_wp_off((hookit_addr)parent) != 0) {
      printf("Hook engine: wp off failed\n");
      return -1;
    }

    while(1) {

      if(base >= (PAGESIZE - len)) {
        break;
      }
	  
      if(parent[0] == 0xe8) {
        printf("Hook engine: found opcode 0x%x at addr 0x%lx\n", parent[0], (hookit_addr)parent);

        buf[0] = parent[1];
        buf[1] = parent[2];
        buf[2] = parent[3];
        buf[3] = parent[4];

        entry->target_original_offset = *(hookit_offset*)buf;
        printf("Hook engine: found offset 0x%x\n", entry->target_original_offset);
		addr = (entry->target_original_offset + (hookit_addr)parent + 5) & (hookit_addr)entry->target;
		printf("Hook engine: found func addr 0x%lx\n", addr);
        if(addr == (hookit_addr)entry->target) {
          goto out;
        }
      }
      base++;
      parent++;
    }
	base = 0;
    if(hookit_wp_on((hookit_addr)(parent - 1)) != 0) {
      printf("Hook engine: wp on failed\n");
      return -1;
    }
	len = (hookit_addr)entry->parent % PAGESIZE;
  }
  
  printf("Hook engine didn't find target func offset\n");
  return -1;
  
  out: {
	hookit_offset offset = 0;
	offset = (hookit_addr)entry->player - (hookit_addr)parent - 5;

	parent[1] = (offset & 0x000000ff);
	parent[2] = (offset & 0x0000ff00) >> 8;
	parent[3] = (offset & 0x00ff0000) >> 16;
	parent[4] = (offset & 0xff000000) >> 24;

	printf("Hook engine: hooked target func.\n");
  }

	return 0;
}
