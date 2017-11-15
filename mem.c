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
#define REGION_SIZE (10*1024)
#define MEM_TYPE_FREE 0
#define MEM_TYPE_ALLOC 1

typedef struct memory {
	int size;
	int type;
	struct memory *next;
}mem;

int static mem_policy = -1;
mem *pointer = NULL;
mem *tempPoint = NULL;

int Mem_Init(int size, int policy)
{
	if (size <= 0)
	{
		return -1;
	}
	if (policy != MEM_POLICY_FIRSTFIT && policy != MEM_POLICY_WORSTFIT && policy != MEM_POLICY_BESTFIT)
	{
		return -1;
	}
	else
	{
		mem_policy = policy;
	}

	// align size with page size
	int page_size = getpagesize();

	printf("Size of region = %d\n", size);
	printf("Page size = %d\n", page_size);

	int diff = size % page_size;
	if (diff != 0)
	{
		size += (page_size - diff);
	}

	printf("Size of region to be allocated = %d\n", size);

	// memory chunk

	int fd = open("/dev/zero", O_RDWR);
	pointer = (mem *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

	if (pointer == NULL)
	{
		return -1;
	}

	close (fd);
	pointer->size = size - sizeof(mem);
	pointer->type = MEM_TYPE_FREE;
	pointer->next = NULL;

	printf("Fragment address: %8x\nsize = %d\n", pointer, pointer->size);

	return 0;
}

void* Mem_Alloc(int size) {
	// Input validtion
	if (size <= 0 || mem_policy == -1)
	{
		return NULL;
	}

	//
}

int Mem_Free(void* ptr) {

}

int Mem_IsValid(void* ptr) {
	tempPoint = pointer;
	do
	{
		if ((*tempPoint).type == 1 && (tempPoint == ptr || ptr == (tempPoint + (*tempPoint).size) || (tempPoint < ptr && (tempPoint + (*tempPoint).size) > ptr)))
		{
			return 1;
		}
		else
		{
			tempPoint = (*tempPoint).next;
		}
	} while ((*tempPoint).next != NULL);
	return 0;
}

int Mem_GetSize(void* ptr) {

}

float Mem_GetFragmentation() {

}

int main(int argc, char* argv[])
{
	printf("init memory allocator...");
	if (Mem_Init(REGION_SIZE, MEM_POLICY_FIRSTFIT) < 0)
	{
		printf("Unable to initialize memory allocator!\n");
		return -1;
	}
	else
	{
		printf("Success!\n");
	}
	return 0;
}
