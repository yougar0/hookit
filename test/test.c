#include <stdio.h>
#include <fcntl.h>
#define _GNU_SOURCE
#include <dlfcn.h>
#include "hookit.h"

#define TARGET "libtarget.so"
#define HOOKIT "libhookit.so"

int my_add(int a, int b) {
  return a - b;
}

int main(int argc, char **argv) {

  void *target = dlopen(SO_NAME, RTLD_NOW);
  if(handle == NULL) {
	return -1;
  }
  
  void *ad = dlsym(target, "add");
  if(ad == NULL) {
	return -1;
  }


  void *add2 = dlsym(target, "add2");
  if(add2 == NULL) return -1;  

  printf("SO Entry:  0x%lx\nadd:  0x%lx\nadd2: 0x%lx\n", (unsigned long)handle, (unsigned long)ad, (unsigned long)add2);
  printf("\nmy_add:  0x%lx\n", (unsigned long)my_add);

  /*
  int (*add)(int, int) = ad;
  printf("add(1, 2) = %d\n", add(1, 2));


    
  if(hookit(hookit_init(add, add2, my_add)) == -1) {
	printf("Hook engine: hook func failed\n");
	return -1;
  }

  printf("add(1, 2) = %d\n", add(1, 2));
  */  

  return 0;
}
