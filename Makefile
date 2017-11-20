#Makefile
current_dir = $(shell pwd)
#export LD_LIBRARY_PATH=/home/cop4610/Desktop/MemAllocator
export LD_LIBRARY_PATH=$(current_dir)
make:
	gcc -c -fpic mem.c
	gcc -shared -o libmem.so mem.o
	gcc main.c -lmem -L. -o myprogram
	export LD_LIBRARY_PATH=$(current_dir)

#For cleaning the files
clean:
	rm -f *.o core
	rm myprogram libmem.so
