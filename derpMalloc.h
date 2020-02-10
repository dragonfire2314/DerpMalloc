/*
	File: derpMalloc.h
	Purpose: A simple and small malloc inplimentaion for derp OS
	Author: Tanner Kern
*/

/*
	Usage: before any functions are used init_malloc() must be called, then malloc(size) will generate memory or size.
	Free(address) will free the memory at that address.

	init_malloc(heapStart, heapSize, minAllocSize)
		-heapStart, Start of the heap
		-heapSize, Maximum size of the heap (can be expanded later)
		-minAllocSize, smallest unit that will be alloced 8, or 16 should be good.
	malloc(size)
		-size, size in bytes that the uses wants to be allocated
	free(loc)
		-loc, the addresss of previously allocated memory
	expandMemory(amount)
		-amount, the amount of memory in bytes that the user wants to expand the heap by
*/

int init_malloc(unsigned char* heapStart, unsigned int heapSize, unsigned int minAllocSize);
void* malloc(unsigned int size);
void free(unsigned char* loc);
void expandMemory(unsigned int amount);