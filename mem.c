#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "mem.h"

//Defines from mem.h:
//MEM_POLICY_FIRSTFIT 0
//MEM_POLICY_BESTFIT  1
//MEM_POLICY_WORSTFIT 2

int mem() {
void *pointer;

    int Mem_Init(int size, int policy) {
	// open the /dev/zero device
	int fd = open("/dev/zero", O_RDWR);
	// size (in bytes) must be divisible by page size
	pointer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (pointer == MAP_FAILED) {
	   perror("mmap");
	   return -1;
	}
	// close the device (don't worry, mapping should be unaffected)
	close(fd);
	
	return pointer;
    }

    void* Mem_Alloc(int size) {
	
    }

    int Mem_Free(void* ptr) {
	
    }

    int Mem_IsValid(void* ptr) {

    }

    int Mem_GetSize(void* ptr) {

    }

    float Mem_GetFragmentation() {

    }

}
