# DerpMalloc
A small malloc implementation for derpOS or any other software that needs to preform malloc on an array with bounds.

## Usage
To start using DerpMalloc the user must first define a pointer to memory that should be used for allocation, then call the init_malloc() functions.
Next, the user may call malloc() to dynamically allocated memory, or free() to release a preveously allocated piece of memory.

## Functions
```C
int init_malloc(unsigned char* heapStart, unsigned int heapSize, unsigned int minAllocSize);
```
