#Makefile
export LD_LIBRARY_PATH=/home/cop4610/Desktop/MemAllocator
make:
	gcc -c -fpic mem.c
	gcc -shared -o libmem.so mem.o
	gcc main.c -lmem -L. -o myprogram

#For cleaning the files
clean:
	rm -f *.o core
	rm myprogram libmem.so

#For cleaning and building the files
rebuild : clean all

