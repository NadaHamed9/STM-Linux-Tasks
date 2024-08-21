#include "functions.h"

/* heap with a large statically allocated array.*/
static uint8_t heap[HEAP_SIZE];

/* points to the end of the last allocated block.*/
static void *program_break = heap;

static Block *head_free = NULL;   /*Head of the free list*/
static Block *tail_free = NULL;   /*Tail of the free list*/

/*points to head of allocated area in sim heap*/
static Block *header=NULL;
/*points to last of allocated area in sim heap*/
static Block *last=NULL;


/*Function to find a free block in the heap*/

Block *find_free_block(size_t size) {
	Block *current = head_free;
	while (current) {
		/*search for free block of needed size or more*/
		if (current->free && current->size >= size) {
			return current;
		}
		current = current->next;
	}
	/*not found freed block at all or of needed size*/
	return NULL;
}
/*function to print free list created*/
void print_freelist() {
        Block *current = head_free;
	if(head_free==NULL)
	{
		printf("no free list\n");
		return;
	}
	printf("Free List:\n");
        while (current!=NULL) {
		
		printf("[Adrr: %p, Size: %ld,Free: %d]\n",current,current->size,current->free);
	        current = current->next;
        }

	printf("the end of list\n");
}

/*function to allocate extra_space free for each HmmAlloc*/
void alloc_extra()
{
	size_t total_size= METADATA_SIZE+EXTRA_SIZE;
	/*check if i can make allocation with this size or not*/
	if((uint8_t*)program_break+total_size > heap+HEAP_SIZE)
	{
		printf("ERROR!! not enough space in heap\n");
		return;
	}

	/*to allocate extra free space after allocated for user*/
	Block *free_block = (Block*)program_break;
	/*mark block as free*/
	free_block->free = 1;
	/*store extra size 1kb*/
	free_block->size=EXTRA_SIZE;
	
	/* Insert the free block into the free list in the correct order */
	if (head_free == NULL || (uint8_t*)free_block < (uint8_t*)head_free) {

		/* Insert at the head of the free list */
		free_block->next = head_free;
		if (head_free != NULL) {
			head_free->prev = free_block;
		}
		/*first node*/
		head_free = free_block;
		free_block->prev=NULL;
		
		if (tail_free == NULL) {
			tail_free = free_block;
		}

	} else {
		
		/* Insert in the correct position */
		Block *current = head_free;
		
		while (current->next &&(uint8_t*) current->next < (uint8_t*)free_block) {
			current = current->next;
		}
		free_block->next = current->next;
		free_block->prev = current;
		current->next = free_block;
		
		if (free_block->next) {
			free_block->next->prev = free_block;
		} else {
			tail_free = free_block;
		}
	}

	/*increment program break*/
	program_break=(uint8_t*)program_break+total_size;

	cleanup_freelist();
}

/*Function to split a block if it's larger than required size*/
void split_block(Block *block, size_t size) {

    if (block->size >= size + METADATA_SIZE + 1) {
	    /* Ensure there is enough space to create a new block*/
	    Block *new_block = (Block *)((uint8_t *)block + METADATA_SIZE + size);
	    /*create new block of size=TOTALsize-(size needed+metadata size)*/
	    new_block->size = block->size - size - METADATA_SIZE;
	    /*mark newblock splitted as free */
	    new_block->free = 1;

	    /*edit free list*/
	    new_block->next = block->next;
	    new_block->prev = block;
	    
	    if (block->next) {
		    block->next->prev = new_block;

	    } 
	    else {
		    tail_free = new_block;
	    }

	    block->size = size;
	    block->next = new_block;
    }

}

/*Function to merge adjacent free blocks*/
void merge_blocks() {

	Block *current = head_free;
	/*loop on free list*/
	while (current) {
		
		Block *nextblock=current->next;

		/*search for two adjacent free blocks*/
		if ((uint8_t*)current +METADATA_SIZE+current->size == (uint8_t*)nextblock 
				&& current->free==1 && nextblock->free==1)
		{
			/*store new size as sum of both blocks*/
			current->size += METADATA_SIZE + nextblock->size;

			current->next = nextblock->next;
			/*edit free list*/	
			if(nextblock->next)
			{
			
				nextblock->next->prev=current;
			}
			else{
				tail_free=current;

			}
		}else{
		current = current->next;
		}
	}
}

/*function to free allocated space area*/
void HmmFree (void* ptr)
{
	/*no area is allocated*/
	if (program_break == heap) {
		printf("ERROR!! Cannot free without allocating\n");
		return;
	}
	/*check on pointer points to valid area*/
	if (ptr == NULL || ptr < (void *)heap || ptr >= (void *)program_break) {
		printf("ERROR!! Pointer is null or locates on area out heap\n");
		return;
	}
	/*to edit on metadata*/
	Block *free_block = (Block*)(ptr) - 1;
	
	/*check on free block*/
        if ((uint8_t*)free_block < heap || (uint8_t*)free_block >= (uint8_t*)program_break) {
                printf("ERROR!! invalid free block\n");
                return;
        }

	/*mark block as free*/
	free_block->free = 1;

	/*check if this free block is last allocated area or not */
	if((uint8_t*)free_block+METADATA_SIZE+free_block->size == (uint8_t*)program_break)
	{
		/*decrement program break*/
		program_break=(uint8_t*)free_block;

		if(program_break==heap)
		{
			head_free=NULL;
			tail_free=NULL;
		}
		/*exit without adding this block to free list*/
		return;
	}
	
	/* Insert the freed block into the free list in the correct order */
	insert_free_block(free_block);

	/*cleanup free list and update it*/
	cleanup_freelist();
}

/*function to insert free block in free list*/
void insert_free_block(Block* free_block)
{
	if(free_block==NULL)
		return;

	/* Insert the freed block into the free list in the correct order */
        if (head_free == NULL ||(uint8_t*) free_block <(uint8_t*) head_free) {
                /* Insert at the head of the free list */
                free_block->next = head_free;
		free_block->prev=NULL;

                if (head_free != NULL) {
                        head_free->prev = free_block;
                }
		else{
                        tail_free = free_block;
                }
		head_free=free_block;

        }
	else {
                /* Insert in the correct position */
                Block *current = head_free;
                while (current->next &&(uint8_t*) current->next < (uint8_t*)free_block) {

                        current = current->next;
                }
		if(current==free_block)
		{
		
			printf("attempt to insert a block next to itself\n");
			return;
		}

                /*edit free list*/
                free_block->next = current->next;
                free_block->prev = current;
                current->next = free_block;

                if(free_block->next){
                        /*if this node in middle of free list*/
                        free_block->next->prev = free_block;
                        
                }else{
                        /*if this node the tail of free list*/
                        tail_free=free_block;
                }
        }
	/*update freelist */
	cleanup_freelist();
}

/*function to keep clean free list from used blocks*/
void cleanup_freelist()
{
	Block*current=head_free;
	while(current)
	{

		if(! current->free)
		{
			/*if this node is used remove it from free list*/
			remove_from_freelist(current);
		}
		current=current->next;
	}

	/*after cleanup merge blocks if found*/
	merge_blocks();
}

/*function to remove node from free list*/
void remove_from_freelist(Block* block)
{
	/*return if block is null*/
	if(block==NULL)
		return;
	/*update head_free if it used*/
	if(block==head_free)
	{
		head_free=block->next;
		if(head_free)
			head_free->prev=NULL;
		else
			tail_free=NULL;

	}else if(block==tail_free)
	{
		tail_free=block->prev;
		if(tail_free)

			tail_free->next=NULL;
		else 
			head_free=NULL;
	}
	else
	{
		block->prev->next=block->next;
		if(block->next)
			block->next->prev=block->prev;
	}
	/*remove this node from list*/
	block->next=NULL;
	block->prev=NULL;
}

/*function to dynamically allocate memory in sim heap*/
void* HmmAlloc(size_t size)
{
	long int total_size=size+METADATA_SIZE;

	/*check if size is appropriate to locate*/
	if(size==0 || total_size>HEAP_SIZE)
	{
		printf("ERROR!! Size is unacceptable\n");
		return NULL;
	}
	Block* block_free=find_free_block(size);

	/*first check if theres free area of this size or not*/
	if(block_free != NULL)
	{
		/*found free block of needed size or more so split if needed*/
		split_block(block_free,size);

		/*mark block as used*/
		block_free->free=0;

		cleanup_freelist();
		/*return user accessible space */
		return (void*)(block_free+1);
	}
	/*check if i can make allocation with this size or not by checking addresses*/
	if((uint8_t*)program_break+total_size > heap+HEAP_SIZE)
	{
		printf("ERROR!! not enough space in heap\n");
		return NULL;
	}
	/*allocate memory with needed space and extra space incase of next allocation*/

	Block* block=(Block*)program_break;

	/*first memory allocation*/
	if(program_break==heap)
	{
		/*store metadata*/
		block->size=size;
		block->free=0;/*mark as used*/
		header=block;
		block->next=NULL;
		block->prev=NULL;
		/*make last=header*/
		last=header;
	}

	/*not first memory allocation*/
	else{ 
		/*store metadata*/
		block->free=0;/*mark as used*/
		block->size=size;
		last->next=block;
		block->prev=last;
		/*update last pointer*/
		last=block;
	}
	/*increment program break*/
	program_break=(uint8_t*)program_break+total_size;

	/*allocate extra free space*/
	alloc_extra();
	/*return accessible user area*/
	return (void*)(block+1);
}

/*function to test merge,update head,split*/
void test1()
{
	void*ptr1=HmmAlloc(400);
        printf("allocated ptr1 of size 400 at: %p\n",ptr1);
	printf("program break at : %p\n",program_break);
	print_freelist();

	void*ptr2=HmmAlloc(100);
        printf("allocated ptr2 of size 100 at: %p\n",ptr2);
	printf("program break at : %p\n",program_break);
	print_freelist();

	void*ptr3=HmmAlloc(300);
        printf("allocated ptr3 of size 300 at: %p\n",ptr3);
	printf("program break at : %p\n",program_break);
	print_freelist();

	/*free block from middle of allocated space then first then last*/
        printf("freed second allocated ptr2 of size 100\n");
        HmmFree(ptr2);
	printf("program break at : %p\n",program_break);
        print_freelist();

	printf("freed first allocated ptr1 of size 400\n");
        HmmFree(ptr1);
        printf("program break at : %p\n",program_break);
	print_freelist();

	printf("freed last allocated ptr3 of size 300\n");
        HmmFree(ptr3);
        printf("program break at : %p\n",program_break);
        print_freelist();

}
/*test normal scenario allocate then free */
void normal()
{
	printf("program break at start at:%p\n",program_break);
	/*allocate ptr1*/
	void*ptr1=HmmAlloc(500);
	printf("allocated ptr1 of size 500 at: %p\n",ptr1);
	printf("program break at: %p\n",program_break);
	print_freelist();

	/*free ptr1*/
	printf("freed first allocated ptr1 of size 500\n");
        HmmFree(ptr1);
        printf("program break at : %p\n",program_break);
        print_freelist();

	/*allocate ptr2 it use of extra space allocated*/
	void*ptr2=HmmAlloc(400);
        printf("allocated ptr2 of size 400 at: %p\n",ptr2);
	printf("program break at: %p\n",program_break);
	print_freelist();

	/*free ptr2*/
	printf("freed last allocated ptr2 of size 400\n");
	HmmFree(ptr2);
	printf("program break at : %p\n",program_break);
	print_freelist();

	/*allocate ptr3*/
	void*ptr3=HmmAlloc(400);
        printf("allocated ptr3 of size 400 at: %p\n",ptr3);
        printf("program break at: %p\n",program_break);
        print_freelist();

}

/*this function for testing*/
void test2()
{
	/*first program break points to start of heap*/
	printf("program break at start: %p\n",program_break);

	void*ptr1=HmmAlloc(100);
	printf("allocated ptr1 of size 100 at: %p\n",ptr1);
        printf("program break at: %p\n",program_break);
        print_freelist();


	/*tries to free area out of heap*/
	int *ptr;
	HmmFree(ptr);

	void*ptr2=HmmAlloc(992);
	printf("allocated ptr2 of size 992 at: %p\n",ptr2);
        printf("program break at: %p\n",program_break);
        print_freelist();

	printf("freed last allocated ptr2 of size 992\n");
        HmmFree(ptr2);
        printf("program break at : %p\n",program_break);
        print_freelist();

	printf("freed first allocated ptr1 of size 100\n");
        HmmFree(ptr1);
        printf("program break at : %p\n",program_break);
        print_freelist();


}
