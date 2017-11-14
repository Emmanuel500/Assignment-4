#Makefile
make:
	gcc -c -fpic mem.c
	gcc -shared -o libmem.so mem.o
	gcc main.c -lmem -L. -o myprogram

#For cleaning the files
clean:
	rm -f *.o core

#For cleaning and building the files
rebuild : clean all

