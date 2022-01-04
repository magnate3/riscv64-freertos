/* Define the start address and size of the three RAM regions. */ 
#define RAM1_START_ADDRESS    ( ( uint8_t * ) 0x00010000 ) 
#define RAM1_SIZE             ( 65 * 1024 ) 

#define RAM2_START_ADDRESS    ( ( uint8_t * ) 0x00020000 ) 
#define RAM2_SIZE             ( 32 * 1024 ) 

#define RAM3_START_ADDRESS    ( ( uint8_t * ) 0x00030000 ) 
#define RAM3_SIZE             ( 32 * 1024 ) 

/* Create an array of HeapRegion_t definitions, with an index for each of the three 
RAM regions, and terminating the array with a NULL address.  The HeapRegion_t 
structures must appear in start address order, with the structure that contains the 
lowest start address appearing first. */ 
const HeapRegion_t xHeapRegions[] = 
{ 
    { RAM1_START_ADDRESS, RAM1_SIZE }, 
    { RAM2_START_ADDRESS, RAM2_SIZE }, 
    { RAM3_START_ADDRESS, RAM3_SIZE }, 
    { NULL,               0         }  /* Marks the end of the array. */ 
}; 

int main( void ) 
{ 
    /* Initialize heap_5. */ 
    vPortDefineHeapRegions( xHeapRegions ); 

    /* Add application code here. */ 
} 