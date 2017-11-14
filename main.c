#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Hello world!\n");

    /*
    int *ptr;
    // The returned memory object is between ptr and ptr+49
    if ((ptr = (int *)Mem_Alloc(50 * sizeof(int))) == NULL)
    exit(1);
    // Could replace 30 with any value from 0 to 49..
    // They have the same effect: reclaiming the region
    // pointed to by ptr
    Mem_Free(ptr+30);
    */

    return 0;
}
