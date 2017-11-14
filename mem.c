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
typedef struct allocated_memory {
	int size;
	int type;
	int policy;
	struct allocated_memory *next;
}alloc_mem;
int mem() {
alloc_mem *pointer;

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
	(*pointer).size = size;
	(*pointer).policy = policy;
	return 0;
    }

    void* Mem_Alloc(int size) {
	
    }

    int Mem_Free(void* ptr) {
	
    }

    int Mem_IsValid(void* ptr) {

    }#include <stdio.h>
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

typedef struct memory {
	int size;
	int type;
	struct memory *next;
}mem;

int static policy = -1;
mem *pointer = NULL;

int Mem_Init(int size, int policy)
{
	if (size <= 0)
	{
		return -1;
	}
	if ((policy < 0) || (policy > 2))
	{
		return -1;
	}
	








	// open the /dev/zero device
	int fd = open("/dev/zero", O_RDWR);
	// size (in bytes) must be divisible by page size
	pointer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (pointer == MAP_FAILED)
	{
	   perror("mmap");
	   return -1;
	}

	// close the device (don't worry, mapping should be unaffected)
	close(fd);
	(*pointer).size = size;
	(*pointer).policy = policy;
	return 0;
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


    int Mem_GetSize(void* ptr) {

    }

    float Mem_GetFragmentation() {

    }

}
