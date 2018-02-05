#include <stdio.h>
#include <fcntl.h>
#define _GNU_SOURCE
#include <dlfcn.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "hookit.h"
#include <unistd.h>

#define TARGET "libtarget.so"
#define HOOKIT "./libhookit.so"

hookit_entry *(*hook_init)(void*, void*, void*);
i8 (*hook)(hookit_entry*);

int add2(int a, int b) {
  return a + b;
}

int add(int a, int b) {
  return add2(a, b);
}


int my_add(int a, int b) {
  return a - b;
}

int main(int argc, char **argv) {
  
  void *hookit_engine = dlopen(HOOKIT, RTLD_NOW);
  if(hookit_engine == NULL) {
	printf("can't load %s\n", HOOKIT);
	return -1;
  }
  
  void *method_hookit = dlsym(hookit_engine, "hookit");
  if(method_hookit == NULL) {
	printf("can't find  hookit method\n");
	return -1;
  }

  void *method_hookit_init = dlsym(hookit_engine, "hookit_init");
  if(method_hookit_init == NULL) {
	printf("can't find hookit_init method\n");
	return -1;  
  }

  hook_init = method_hookit_init;
  hook = method_hookit;
  
  hook(hook_init(add, add2, my_add));
  
  printf("at parent: add: 1 + 2 = %d\n", add(1, 2));

  pid_t child_pid;
  child_pid = fork();
  if(child_pid == -1) {
	perror("fork child failed");
	return -1;
  }

  if(child_pid == 0) {
	printf("at child: add: 1 + 2 = %d\n", add(1, 2));
	hook(hook_init(add, my_add, add2));
	_exit(EXIT_SUCCESS);
  } else {
	wait(NULL);
	printf("at parent: add 1 + 2 = %d\n", add(1, 2));                /* Wait for child */
    exit(EXIT_SUCCESS);
  }
  
  /*
  int (*add)(int, int) = ad;
  printf("add(1, 2) = %d\n", add(1, 2));


    
  if(hookit(hookit_init(add, add2, my_add)) == -1) {
	printf("Hook engine: hook func failed\n");
	return -1;
  }

  printf("add(1, 2) = %d\n", add(1, 2));
  */  

}
