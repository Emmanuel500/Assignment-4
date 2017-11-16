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

typedef struct header {
	int freeSize;
	struct memory *current;
	struct memory *found;
	struct memory *first;
}head;

int static mem_policy = -1;
head *pointer = NULL;
int Mem_Init(int size, int policy)
{
	// Check for initialization
	if (mem_policy != -1)
	{
		return -1;
	}
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
	int diff = size % page_size;
	if (diff != 0)
	{
		size += (page_size - diff);
	}
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

	return 0;
}

void* Mem_Alloc(int size)
{
	// Check for initialization
	if (mem_policy == -1)
	{
		return NULL;
	}
	// Check for valid input parameters
	if (size <= 0 || mem_policy == -1)
	{
		return NULL;
	}
	if (size > pointer->freeSize)
	{
		return NULL;
	}
	pointer->current = pointer->first;
	pointer->found = NULL;
	// Check each free fragment for valid sizes
	while (pointer->current != NULL)
	{
		// Finds the first valid free fragment
		if (mem_policy == MEM_POLICY_FIRSTFIT)
		{
			if (pointer->current->type == MEM_TYPE_FREE)
			{
				if (pointer->current->size > size)
				{
					pointer->found = pointer->current;
					break;
				}
			}
		}
		// Finds the smallest valid free fragmen
		else if (mem_policy == MEM_POLICY_BESTFIT)
		{
			if (pointer->current->type == MEM_TYPE_FREE)
			{
				if (pointer->current->size > size && pointer->found == NULL)
				{
					pointer->found = pointer->current;
				}
				else if (pointer->current->size > size && pointer->found->size > pointer->current->size)
				{
					pointer->found = pointer->current;
				}
			}
		}
		// Finds the largest valid free fragment
		else if (mem_policy == MEM_POLICY_WORSTFIT)
		{
			if (pointer->current->type == MEM_TYPE_FREE)
			{
				if (pointer->current->size > size && pointer->found == NULL)
				{
					pointer->found = pointer->current;
				}
				else if (pointer->current->size > size && pointer->current->size > pointer->found->size)
				{
					pointer->found = pointer->current;
				}
			}
		}
		pointer->current = pointer->current->next;
	}
	// Check if the right node was ever found
	if(pointer->found == NULL)
	{
		return NULL;
	}
	// Allocate memory and change free memory size
	pointer->current = pointer->found->next;
	pointer->found->type = MEM_TYPE_ALLOC;
	pointer->found->next = pointer->found + size + 1;
	pointer->found->next->type = MEM_TYPE_FREE;
	pointer->found->next->next = pointer->current;
	pointer->found->next->size = pointer->found->size - size;
	pointer->found->size = size;
	pointer->freeSize -= size;
	return (void *)pointer->found;
}

int Mem_Free(void* ptr)
{
	if (ptr == NULL)
	{
		return -1;
	}
	pointer->current = pointer->first;
	while (pointer->current != NULL)
	{
		if (pointer->current->type == MEM_TYPE_ALLOC && (mem *)ptr >= pointer->current && (mem *)ptr <= pointer->current + pointer->current->size)
		{
			pointer->current->type = MEM_TYPE_FREE;
			pointer->freeSize += pointer->current->size;
			return 0;
		}
		else if ((pointer->current->type == MEM_TYPE_FREE) && ((mem *)ptr >= pointer->current && (mem *)ptr <= (pointer->current + pointer->current->size)))
		{
			return -1;
		}
		pointer->current = pointer->current->next;
	}
	return -1;
}

int Mem_IsValid(void* ptr)
{
	pointer->current = pointer->first;
	while (pointer->current != NULL)
	{
		if ((pointer->current->type == MEM_TYPE_ALLOC) && ((mem *)ptr >= pointer->current && (mem *)ptr <= (pointer->current + pointer->current->size)))
		{
			return 1;
		}
		else if ((pointer->current->type == MEM_TYPE_FREE) && ((mem *)ptr >= pointer->current && (mem *)ptr <= (pointer->current + pointer->current->size)))
		{
			return 0;
		}
		pointer->current = pointer->current->next;
	}
	return 0;
}

int Mem_GetSize(void* ptr)
{
	pointer->current = pointer->first;
	while (pointer->current != NULL)
	{
		if ((pointer->current->type == MEM_TYPE_ALLOC) && ((mem *)ptr >= pointer->current && (mem *)ptr <= (pointer->current + pointer->current->size)))
		{
			return pointer->current->size;
		}
		else if ((pointer->current->type == MEM_TYPE_FREE) && ((mem *)ptr >= pointer->current && (mem *)ptr <= (pointer->current + pointer->current->size)))
		{
			return -1;
		}
		pointer->current = pointer->current->next;
	}
	return -1;
}

float Mem_GetFragmentation()
{
	pointer->current = pointer->first;
	pointer->found = NULL;
	while (pointer->current != NULL)
	{
		if (pointer->current->type == MEM_TYPE_FREE && pointer->found == NULL)
		{
			pointer->found = pointer->current;
		}
		else if (pointer->current->type == MEM_TYPE_FREE && pointer->current->size > pointer->found->size)
		{
			pointer->found = pointer->current;
		}
		pointer->current = pointer->current->next;
	}
	return ((float)pointer->found->size/((float)pointer->freeSize));
}
