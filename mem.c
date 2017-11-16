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

typedef struct header {
	int freeSize;
	struct memory *first;
}head;

int static mem_policy = -1;
head *pointer = NULL;

int Mem_Init(int size, int policy)
{
	// Check for valid input parameters and set policy
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

	// Adjust requested size to a multiple of page size
	int page_size = getpagesize();
	printf("Requested memory size = %d\n", size);
	printf("Page size = %d\n", page_size);
	int diff = size % page_size;
	if (diff != 0)
	{
		size += (page_size - diff);
	}
	printf("Adjusted memory size = %d\n", size);
	int fd = open("/dev/zero", O_RDWR);
	pointer = (head *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (pointer == NULL)
	{
		return -1;
	}
	close (fd);

	// Set inital struct attributes
	pointer->freeSize = size - sizeof(*pointer);
	pointer->first = (mem *)pointer + sizeof(*pointer);
	pointer->first->size = pointer->freeSize;
	pointer->first->type = MEM_TYPE_FREE;
	pointer->first->next = NULL;

	//printf("Head address: %8x\nTotal size = %d\n", pointer, pointer->freeSize);
	//printf("Size of = %d\n", sizeof(*pointer));
	//printf("Free address: %8x\nFragment size = %d\n", pointer->first, pointer->first->size);

	return 0;
}

void* Mem_Alloc(int size)
{
	// Check for valid input parameters
	if (size <= 0 || mem_policy == -1)
	{
		return NULL;
	}
	if (size > pointer->freeSize)
	{
		return NULL;
	}

	mem *current = pointer->first;
	mem *found = NULL;

	// Check each free fragment for valid sizes
	while (current != NULL)
	{
		// Finds the first valid free fragment
		if (mem_policy == MEM_POLICY_FIRSTFIT)
		{
			if (current->type == MEM_TYPE_FREE)
			{
				if (current->size > size)
				{
					found = current;
					//printf("First address: %8x\nFragment size = %d\n", current, current->size);
					break;
				}
			}
		}
		// Finds the smallest valid free fragment
		else if (mem_policy == MEM_POLICY_BESTFIT)
		{
			if (current->type == MEM_TYPE_FREE)
			{
				if (current->size > size && found == NULL)
				{
					found = current;
					//printf("Best address: %8x\nFragment size = %d\n", current, current->size);
				}
				else if (current->size > size && found->size > current->size)
				{
					found = current;
					//printf("Best address: %8x\nFragment size = %d\n", current, current->size);
				}
			}
		}
		// Finds the largest valid free fragment
		else if (mem_policy == MEM_POLICY_WORSTFIT)
		{
			if (current->type == MEM_TYPE_FREE)
			{
				if (current->size > size && found == NULL)
				{
					found = current;
					//printf("Worst address: %8x\nFragment size = %d\n", current, current->size);
				}
				else if (current->size > size && current->size > found->size)
				{
					found = current;
					//printf("Worst address: %8x\nFragment size = %d\n", current, current->size);
				}
			}
		}
		current = current->next;
	}
	// Check if the right node was ever found
	if(found == NULL)
	{
		printf("Unable to allocate size = %d\n", size);
		return NULL;
	}

	// Allocate memory and change free memory size
	found->type = MEM_TYPE_ALLOC;
	found->next = found + size + 1; //
	found->next->type = MEM_TYPE_FREE;
	found->next->next = NULL;
	found->next->size = found->size - size; //
	found->size = size;

	pointer->freeSize -= size; //

	printf("Allocated address: %8x\nFragment size = %d\n", found, found->size);
	printf("Size of = %d\n", sizeof(*found));

	return (void*)found;
}

int Mem_Free(void* ptr)
{
	mem* current = pointer->first;
	while (current != NULL)
	{
		if ((current->type == MEM_TYPE_ALLOC) && ((mem*)ptr >= current && (mem*)ptr <= (current + current->size)))
		{
			current->type = 0;
			printf("FREE\n");
			return 0;
		}
		current = current->next;
	}
	printf("Nothing to FREE\n");
	return -1;
}

int Mem_IsValid(void* ptr)
{
	mem* current = pointer->first;
	while (current != NULL)
	{
		if ((current->type == MEM_TYPE_ALLOC) && ((mem*)ptr >= current && (mem*)ptr <= (current + current->size)))
		{
			printf("Here\n");
			return 1;
		}
		current = current->next;
	}
	printf("Nope\n");
	return 0;
}

int Mem_GetSize(void* ptr) {
	mem* current = pointer->first;
	while (current != NULL)
	{
		if ((current->type == MEM_TYPE_ALLOC) && ((mem*)ptr >= current && (mem*)ptr <= (current + current->size)))
		{
			printf("Size: %d\n", current->size);
			return current->size;
		}
		current = current->next;
	}
	printf("No object to get size of\n");
	return -1;
}

float Mem_GetFragmentation() {

}
