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
#define MEM_TYPE_FREE 0
#define MEM_TYPE_ALLOC 1

typedef struct memory {
	int size;
	int type;
	struct memory *next;
}mem;

int static mem_policy = -1;
mem *pointer = NULL;

int Mem_Init(int size, int policy)
{
	if (size <= 0)
	{
		return -1;
	}
	if ((policy < MEM_POLICY_FIRSTFIT) || (policy > MEM_POLICY_WORSTFIT))
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
	head_node = (mem *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

	if (head_node == NULL)
	{
		return -1;
	}

	close (fd);
	head_node->size = size - sizeof(mem);
	head_node->type =
	head_node->next = NULL;

	printf("memory chunk address: %8x\nsize = %d\n", head_node, head_node->size);

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
