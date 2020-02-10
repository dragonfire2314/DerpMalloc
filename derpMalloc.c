/*
	File: derpMalloc.c
	Purpose: A simple and small malloc inplimentaion for derp OS
	Author: Tanner Kern
*/

#define HEADER_SIZE sizeof(Malloc_Memory)
#define NULL 0

typedef struct
{
	unsigned int size;
	unsigned char free; //1 - free, 0 - used
	struct Malloc_Memory* next;
} Malloc_Memory;

Malloc_Memory* malloc_node();

//Start node
Malloc_Memory* head_node = 0;
Malloc_Memory* current_node;
//Size
unsigned char* heapPointer = 0x0;
unsigned char* heapBase = 0x0;
unsigned int maxHeapSize = 0;
unsigned int minMemSize = 16;

int init_malloc(unsigned char* heapStart, unsigned int heapSize, unsigned int minAllocSize)
{
	minMemSize = minAllocSize;
	heapPointer = heapStart;
	heapBase = heapStart;
	maxHeapSize = heapSize;
	if (heapSize < 1024) { return -1; } //Return if heap is below 1024 bytes in size (This can be disabled manually)

	//Create start node of size 0
	head_node = (Malloc_Memory*)heapStart;
	head_node->size = 0;
	head_node->free = 0;
	head_node->next = NULL;
	heapPointer += HEADER_SIZE;
}

Malloc_Memory* addNode(unsigned int size)
{
	current_node = head_node;
	//Find end of node list
	while (current_node->next != NULL)
	{
		current_node = (Malloc_Memory*)current_node->next;
	}
	//On the last node
	//Create a node
	Malloc_Memory* new_node = malloc_node();
	//Point current at new
	current_node->next = (struct Malloc_Memory*)new_node;
	//Set node size
	new_node->size = size;
	//Return new node
	return new_node;
}

void removeNode(unsigned int* address)
{
	current_node = head_node;
	//Look for node
	while (current_node->next != address)
	{
		current_node = current_node->next;
	}
	//node found
	Malloc_Memory* temp = current_node->next;
	current_node->next = temp->next;
	current_node->size += (temp->size + HEADER_SIZE);
}

Malloc_Memory* malloc_node()
{
	Malloc_Memory* temp = (Malloc_Memory*)heapPointer;
	heapPointer += HEADER_SIZE;

	temp->free = 0;
	temp->next = 0;

	return temp;
}

unsigned char* alloc_heap(unsigned int size)
{
	unsigned char* temp = heapPointer;
	heapPointer += size;
	return temp;
}

//Cleans up the linked list by combining free ones that are next to one anouther
void cleanMemoryList()
{
	current_node = head_node;
	//Loop till end of list is hit and return is called
	while (current_node->next != NULL)
	{
		//Shearch for freed memory
		while (current_node->free != 1 && current_node->next != NULL) {
			current_node = current_node->next;
		}
		//Found free memory
		//Check next node for free mem
		if (current_node->next != NULL) {
			Malloc_Memory* temp = current_node->next;
			if (temp->free == 1) {
				removeNode(temp);
			}
			else {
				current_node = temp;
			}
		}
		else { //Hit end of list
			return;
		}
	}
}

void expandMemory(unsigned int amount)
{
	maxHeapSize += amount;
}

void* malloc(unsigned int size)
{
	//Clean the linked list (combine free nodes)
	cleanMemoryList();
	//Check if size is large enough, if not add to size
	if (size <= minMemSize) {
		size = minMemSize;
	}
	//Check for free memory
	current_node = head_node;
	while (current_node->next != NULL) {
		//Check for a free node
		if (current_node->free == 1) {
			//Check if node is big enough
			if (current_node->size > size) {
				//Check if node should be broken apart
				if ((HEADER_SIZE + minMemSize + size) < current_node->size) {
					//Alloc data
					int oldSize = current_node->size;
					Malloc_Memory* returnData = (unsigned char*)current_node + HEADER_SIZE;
					current_node->size = size;
					current_node->free = 0;
					//Insert a node
					Malloc_Memory* temp = current_node->next;
					current_node->next = ((unsigned char*)current_node + HEADER_SIZE + size);
					current_node = current_node->next;
					current_node->next = temp;
					current_node->free = 1;
					current_node->size = (oldSize - HEADER_SIZE - size);
					//Return data
					return returnData;
				}
			}
		}
		current_node = current_node->next;
	}

	//Check if memory is avalible
	if (!((size + HEADER_SIZE) < ((heapBase + maxHeapSize) - heapPointer))) {
		expandMemory();
	}
	//Create new node and data
	addNode(size);
	//Alloc data
	return alloc_heap(size);
}

void free(unsigned char* loc)
{
	//Label memory as free
	Malloc_Memory* temp = (loc - HEADER_SIZE);
	temp->free = 1;
}