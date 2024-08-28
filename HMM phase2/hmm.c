#include "functions.h"

/*pointer to track heap allocations.*/
static void *program_break = NULL;

static Block *head_free = NULL;   /*Head of the free list*/
static Block *tail_free = NULL;   /*Tail of the free list*/

/*points to head of allocated area in heap*/
static Block *header=NULL;
/*points to last of allocated area in heap*/
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

	/*allocate extra space in heap by incrementing program break*/
	void *req=sbrk(total_size);

	/*check if sbrk fails*/
	if(req == (void*) -1)
	{
		printf("ERROR!! not enough space sbrk failes\n");
		return;
	}

	/*to allocate extra free space after allocated for user*/
	Block *free_block = (Block*)req;
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
	program_break=(uint8_t*)req+total_size;	
}

/*Function to split a block if it's larger than required size*/
void split_block(Block *block, size_t size) {
	
	/* Ensure there is enough space to create a new block*/
	if (block->size >= size + METADATA_SIZE + 1) {
		
		Block *new_block = (Block *)((uint8_t *)block + METADATA_SIZE + size);
		/*create new block of size=TOTALsize-(size needed+metadata size)*/
		new_block->size = block->size - size - METADATA_SIZE;
		/*mark newblock splitted as free */
		new_block->free = 1;
		
		/*edit free list*/
		new_block->next = block->next;
	        new_block->prev = block;
		/*if newblock created not the last block*/
		if (block->next) {
			block->next->prev = new_block;
		}
		else {
			/*if it last block updates tail of free list*/
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

	/*check on pointer points to valid area*/
	if (ptr == NULL) {
		//printf("ERROR!! Pointer is null or locates on area out heap\n");
		return;
	}
	/*no area is allocated to be freed*/
        if (program_break == NULL) {
                printf("ERROR!! Cannot free without allocating\n");
                return;
        }


	/*to edit on metadata*/
	Block *free_block = (Block*)(ptr) - 1;

	/*mark block as free*/
	free_block->free = 1;

	/*check if this free block is last allocated area or not */
	if((uint8_t*)free_block+METADATA_SIZE+free_block->size == (uint8_t*)program_break)
	{
		/*decrement program break in heap*/
		intptr_t decrement= -(intptr_t)(METADATA_SIZE+free_block->size);

		program_break=(uint8_t*)free_block;

		void*result=sbrk(decrement);
		/*checks on sbrk */
		if(result==(void*)-1)
		{
			printf("unable to decrement program break\n");
		}
		else{
			 program_break=(uint8_t*)result;
			/*thats mean every allocated area is freed so remove free list*/
			if(program_break==header)
			{
				head_free=NULL;

				tail_free=NULL;
				printf("kol allocated free\n");
			}
		}

		/*exit without adding this block to free list*/
		return;
	}
	
	/* Insert the freed block into the free list in the correct order */
	insert_free_block(free_block);
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
			/*attempt to insert a block next to itself*/
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
	/*update freelist and merge if needed*/
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
	if(size==0)
	{
		printf("ERROR!! Size is zero\n");
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
	/*allocate area in heap*/
	void* req=sbrk(total_size);
	/*check if sbrk fails*/
	if(req ==(void*) -1)
	{
		printf("ERROR!! not enough space sbrk failed\n");
		return NULL;
	}

	/*allocate memory with needed space and extra space incase of next allocation*/
	Block* block=(Block*)req;

	/*first memory allocation*/
	if(program_break==NULL)
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
	program_break=(uint8_t*)req+total_size;

	/*allocate extra free space*/
	alloc_extra();

	/*update freelist and clean it and merge if needed*/
	cleanup_freelist();

	/*return accessible user area*/
	return (void*)(block+1);
}

/*actual malloc*/
void *malloc(size_t size)
{
	/*do allign if needed*/
	size=allign_size(size);

	if(size ==0)
		return HmmAlloc(24);
	else
		return HmmAlloc(size);
}

/*actual free*/
void free(void *ptr)
{
	HmmFree(ptr);
}

/*actual calloc*/
void *calloc(size_t nmemb, size_t size)
{
	/*do allign if needed*/
        size=allign_size(size);

	void* ptr=HmmAlloc(nmemb*size);

	if(ptr)
	{
		/*intialliaze allocated memory by 0*/
		memset(ptr,0,nmemb*size);
	}
			
	return ptr;
}

/*actual realloc*/
void *realloc(void *ptr, size_t size)
{
#if ( DEBUGGING == 1)
	
	printf("realloc adress %p with size %ld\n",ptr,size);
#endif

	/*do allign if needed*/
        size=allign_size(size);

	if(ptr==NULL && size !=0)
	{
		return HmmAlloc(size);
	}
	else if(size==0 && ptr != NULL)
	{
		HmmFree(ptr);
		return NULL;
	}
	else if(size==0 && ptr ==NULL)
		return HmmAlloc(0);
	else { 
		/*to edit on metadata*/
		Block *block = (Block*)(ptr) - 1;

		/*allocate area with needed size*/
		void *newptr=HmmAlloc(size);

		/*check ptr not equal null*/
		if(ptr && newptr){
			/*reallocate large size then copy old data*/
			if(size > block->size)
			{
				/*copy data from old location to new one*/
				memcpy(newptr,ptr,block->size);
				/*free old location*/
				HmmFree(ptr);

				return newptr;
			}
			/*new location is small than old one*/
			else if(size < block->size)
			{
				/*copy data that suitable for ne size*/
				memcpy(newptr,ptr,size);
				/*free old location*/
				HmmFree(ptr);

				return newptr;
			}
			/*undifined behaviour do nothing*/
			else 
				return ptr;
		}
	}
}
/*allignment function*/
size_t allign_size(size_t size)
{
	/*no need for allignment*/
	if(!( size % 8)) 
		return size;
	else{
		/*allign size to 8*/
		size = 8*((size+7)/8);
		return size;
	}
}



