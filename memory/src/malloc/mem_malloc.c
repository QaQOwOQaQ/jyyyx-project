#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "mem_malloc.h"

/* use byte array to simulate heap */
#define PAGE_SIZE       (4096)   
#define HEAP_MAX_SIZE   (4096 * 8)    // max malloc 8 physical pages

uint8_t heap[HEAP_MAX_SIZE];
uint64_t heap_start_vaddr = 4; // for debug , (1)
uint64_t heap_end_vaddr = PAGE_SIZE - 1;


/*   main function   */
int heap_init(); 
uint64_t mem_alloc(uint32_t size); // malloc and return payload
void mem_free(uint64_t varddr);

/* simulate brk() */
void os_syscall_brk(uint64_t)
{  
    heap_end_vaddr += PAGE_SIZE;
}

/* some help function */
static uint64_t round_up(uint64_t x, uint64_t n); // return new x which is n bytes aligning

static uint32_t get_blocksize(uint64_t header_vaddr);
static void set_blocksize(uint64_t header_vaddr, uint32_t blocksize);

static uint32_t get_allocated(uint64_t header_vaddr);
static void set_allocated(uint64_t header_vaddr, uint32_t allocated);

static int32_t is_lastblock(uint64_t vaddr);

static uint64_t get_payload_addr(uint64_t vaddr);
static uint64_t get_header_addr(uint64_t vaddr);
static uint64_t get_footer_addr(uint64_t vaddr);

static uint64_t get_nextheader(uint64_t vaddr);
static uint64_t get_prevheader(uint64_t vaddr);

// Round up to next multiple of n:
// if (x == k * n), (k = 0, 1, 2, ...)
// return x
// else, x = k * n + m and m < n
// return (k + 1) * n
static uint64_t round_up(uint64_t x, uint64_t n)
{
    return ((x + n - 1) / n) * n;
}

// applicapable for both header & footer
static uint32_t get_blocksize(uint64_t header_vaddr)
{
    if(header_vaddr == 0)
    {
        return 0;
    }

    assert(header_vaddr >= heap_start_vaddr && header_vaddr <= heap_end_vaddr);
    assert((header_vaddr & 0x3) == 0x0);    // header & footer should be 4 bytes aligned

    // use this sentense, we can get 4 bytes in header_value
    // we use x to alias header_vaddr
    // that: 0x heap[x+3] heap[x+2] heap[x+1] heap[x], (small endian)
    uint32_t header_value = *(uint32_t *)&heap[header_vaddr];
    return (header_value & 0xFFFFFFFE); 
}

// applicapable for both header & footer
static void set_blocksize(uint64_t header_vaddr, uint32_t blocksize)
{
    if(header_vaddr == 0)
    {
        return ;
    }
    assert(header_vaddr >= heap_start_vaddr && header_vaddr <= heap_end_vaddr);
    assert((header_vaddr & 0x3) == 0x0);    // header & footer should be 4 bytes aligned
    assert((blocksize & 0x7) == 0x0);   // block size should be 8 bytes aligned
    // for last block, the virtual block size still 8n
    // we imagine there is a footer in another physical page
    // but it actually does not exist
    // (2)

    *(uint32_t*)&heap[header_vaddr] &= 0x00000007; // clear size(not change lowest three bits)
    *(uint32_t*)&heap[header_vaddr] |= blocksize;
}

// applicapable for both header & footer
static uint32_t get_allocated(uint64_t header_vaddr)
{
    if(header_vaddr == 0)
    {
        // NULL can be considered as allocated
        // so when we search free block will ignore it
        return 1;
    }

    assert(header_vaddr >= heap_start_vaddr && header_vaddr <= heap_end_vaddr);
    assert((header_vaddr & 0x03) == 0x0); // header & footer should be 4 bytes aligned

    uint32_t header_value = *(uint32_t*)&heap[header_vaddr];
    return (header_value & 0x1);
}

// applicapable for both header & footer
static void set_allocated(uint64_t header_vaddr, uint32_t allocated)
{
    if(header_vaddr == 0)
    {
        return ;
    }
    assert(header_vaddr >= heap_start_vaddr && header_vaddr <= heap_end_vaddr);
    assert((header_vaddr & 0x3) == 0x0);    // header & footer should be 4 bytes aligned
    
    *(uint32_t*)&heap[header_vaddr] &= 0xFFFFFFFE; // clear allocated bit
    *(uint32_t*)&heap[header_vaddr] |= (allocated & 0x1);   // not |= allocated, because allocated is uint32_t
}

// return 1 if is the last block, else return 0
static int is_lastblock(uint64_t vaddr)
{
    if(vaddr == 0)
    {
        return 0;
    }

    // vaddr can be:
    // 1. sstart address of the block(header) (8 * n + 4)
    // 2. starting address of the payload (8 * m)
    assert(vaddr >= heap_start_vaddr && vaddr <= heap_end_vaddr);
    assert((vaddr & 0x3) == 0x0);

    uint64_t header_vaddr = get_header_addr(vaddr);
    uint32_t blocksize = get_blocksize(header_vaddr);

    // for last block, the virtual block size is still 8n
    // we imagine there is a footer in another physical page
    // but it actually does not exist
    if(header_vaddr + blocksize == heap_end_vaddr + 1 + 4)      // (3)
    {
        // it is the last block
        // it does not have any footer
        return 1;
    }

    // not, it's not the last footer
    // it should have footer
    return 0;
}

static uint64_t get_payload_addr(uint64_t vaddr)
{
    // vaddr can be:
    // 1. starting address of the block (8 * n + 4)
    // 2. starting address of the payload (8 * m)
    return round_up(vaddr, 8);
}

static uint64_t get_header_addr(uint64_t vaddr)
{
    // vaddr can be:
    // 1. sstart address of the block(header) (8 * n + 4)
    // 2. starting address of the payload (8 * m)
    uint64_t payload_vaddr = get_payload_addr(vaddr);

    // NULL block does not have header
    return payload_vaddr == 0 ? 0 : payload_vaddr - 4;
}

static uint64_t get_footer_addr(uint64_t vaddr)
{
    // vaddr can be:
    // 1. starting address of the block (8 * n + 4)
    // 2. starting address of the payload (8 * m)
    uint64_t next_header = get_nextheader(vaddr);

    // last block does not have footer
    return next_header == 0 ? 0 : next_header - 4;
}

static uint64_t get_nextheader(uint64_t vaddr)
{
    if(vaddr == 0 || is_lastblock(vaddr))
    {
        return 0;
    }
    assert(vaddr >= heap_start_vaddr && vaddr <= heap_end_vaddr);

    // vaddr can be:
    // 1. starting address of the block (8 * n + 4)
    // 2. starting address of the payload (8 * m)
    uint64_t header_vaddr = get_header_addr(vaddr);
    uint64_t blocksize = get_blocksize(header_vaddr);

    uint64_t next_header_vaddr = header_vaddr + blocksize;
    assert(next_header_vaddr >= heap_start_vaddr && next_header_vaddr <= heap_end_vaddr);
    
    return next_header_vaddr;
}

static uint64_t get_prevheader(uint64_t vaddr)
{
    if(vaddr == 0)
    {
        return 0;
    }

    // vaddr can be:
    // 1. starting address of the block (8 * n + 4)
    // 2. starting address of the payload (8 * m)
    uint64_t header_vaddr = get_header_addr(vaddr); 
    if(header_vaddr == heap_start_vaddr)
    {
        // this block is the first block in heap
        return 0;
    }

    uint64_t prev_footer_vaddr = header_vaddr - 4;
    uint32_t prev_blocksize = get_blocksize(prev_footer_vaddr);

    uint64_t prev_header_addr = header_vaddr - prev_blocksize;
    assert(prev_header_addr >= heap_start_vaddr && prev_header_addr <= heap_end_vaddr - 8 - 4);

    return prev_header_addr;
}


/*========================*/
/*   Explicit Free list   */
/*========================*/
static uint32_t get_nextfree(uint64_t header_addr)
{
    return *(uint32_t *)&heap[header_addr + 8];
}

static uint32_t get_prevfree(uint64_t header_addr)
{
    return *(uint32_t *)&heap[header_addr + 4];
}


/*  Mainly function  */
int heap_init()
{
    // reset all to 0
    for(int i = 0; i < HEAP_MAX_SIZE/ 8; i += 8)
    {
        *(uint64_t*)&heap[i] = i;
    }

    // heap_start_vaddr is the starting address of the first block
    // the payload of the first block is 8B aligned ([8])
    // so the header address of the first block is [8] - 4 = [4]
    heap_start_vaddr = 4;

    // set the header of the only one block
    set_blocksize(heap_start_vaddr, 4096 - 8);
    set_allocated(heap_start_vaddr, 0);

    // we do not set footer for the last block in heap
    
    heap_end_vaddr = 4096 - 1;

    return 0;
}

// try to malloc size(request_blocksize) 
// from the block which start at block_vaddr
// request_blocksize container header, footer, payload
// return - payload_addr if can allocated, else 0
static uint64_t try_alloc(uint64_t block_vaddr, uint32_t request_blocksize)
{
    uint64_t b = block_vaddr;

    uint32_t b_blocksize = get_blocksize(b);
    uint32_t b_allocated = get_allocated(b);

    // not allocated and have the enough space
    if(b_allocated == 0 && b_blocksize >= request_blocksize)
    {   
        // allocated this block
        if(b_blocksize > request_blocksize)
        {
        // split a block from block 'b' provided to request
            set_allocated(b, 1);
            set_blocksize(b, request_blocksize);

            // set the left splitted block
            // in the extreme sitiation, next block size == 8
            // which make he whole block of next to be:
            // [0x00000008, 0x00000008]
            uint64_t next_header_vaddr = b + request_blocksize;
            set_allocated(next_header_vaddr, 0);
            set_blocksize(next_header_vaddr, b_blocksize - request_blocksize);
    
            return get_payload_addr(b);
        }
        else 
        {
            // no nedd to split this block
            // set blocksize(b, request_blockzie)
            set_allocated(b, 1);
            return get_payload_addr(b);
        }
    }

    // cant allocated
    return 0;
}


// size - request padload size
// return - the virtual address of payload
uint64_t mem_alloc(uint32_t size)
{
    // we restricted the max size we can malloc is a physical page
    // because we need to set header and footer
    // so the max size we can alloc is physical_page_size - 8
    assert(size > 0 && size < PAGE_SIZE - 8);

    uint64_t payload_vaddr = 0;
    uint32_t request_blocksize = round_up(size, 8) + 4 + 4;

    uint64_t last_block = 0;
    uint64_t b = heap_start_vaddr;

    // travel all the blocks 
    // it is low-effencience because we would search the block which is allocated
    while(b <= heap_end_vaddr)
    {
        payload_vaddr =  try_alloc(b, request_blocksize);
        if(payload_vaddr != 0)
        {
            return payload_vaddr;
        }
        else    
        {
            // go to the next block

            // if b is the last block
            // we need to alloc a new physical page
            // and add footer to the last block currently
            if(is_lastblock(b))
            {
                last_block = b;
                break;  
            }
            b = get_nextheader(b);  // Implicit
            // b = get_nextfree(b); // Explicit
        }
    }

    // when not enough free block for current heap
    // request a new free physical & virtual page from OS
    if(heap_end_vaddr + 1 + PAGE_SIZE <= HEAP_MAX_SIZE)
    {
        // we can allocate one page fro the request
        uint64_t old_heap_end_vaddr = heap_end_vaddr;

        // brk system call
        os_syscall_brk(heap_end_vaddr + 1);

        uint32_t last_allocated = get_allocated(last_block);
        uint32_t last_blocksize = get_blocksize(last_block);

        if(last_allocated == 1)
        {
            // no merging is needed

            // add footer for last block
            set_allocated(old_heap_end_vaddr, 1);
            set_blocksize(old_heap_end_vaddr, last_blocksize);

            set_allocated(old_heap_end_vaddr + 4, 0);
            set_blocksize(old_heap_end_vaddr + 4, PAGE_SIZE);   

            // update the last block
            last_block = old_heap_end_vaddr + 4;
        }
        else 
        {
            // merging with last lbock is needed
            set_blocksize(last_block, last_blocksize + PAGE_SIZE);
        }

        // try to allocate
        payload_vaddr = try_alloc(last_block, request_blocksize);
        if(payload_vaddr != 0)
        {
            return payload_vaddr;
        }
        else 
        {
            printf("OS cant allocate a new physical page for heap!\n");
        }
    }

    // <==> return NULL;
    return 0;
}

void mem_free(uint64_t payload_vaddr)
{
    assert(payload_vaddr >= heap_start_vaddr && payload_vaddr <= heap_end_vaddr);
    assert((payload_vaddr & 0x7) == 0x0);

    // request can be first or last block
    uint64_t req = get_header_addr(payload_vaddr);
    uint64_t req_footer = get_footer_addr(req); // for last block, it's 0

    uint32_t req_allocated = get_allocated(req);
    uint32_t req_blocksize = get_blocksize(req);
    assert(req_allocated == 1);

    // block starting address of next & prev blocks
    uint64_t next = get_nextheader(req);    // for req last block, it's 0
    uint64_t next_footer = get_footer_addr(next);
    
    uint64_t prev = get_prevheader(req);    // for req first block, it's 0

    uint32_t next_allocated = get_allocated(next);  // for req last, 1
    uint32_t prev_allocated = get_allocated(prev);  // for req first, 1
    
    uint64_t next_blocksize = get_blocksize(next);  // for req lsat, 0
    uint64_t prev_blocksize = get_blocksize(prev);  // for req lsat, 0

    // do not need to set blocksize
    if(next_allocated == 1 && prev_allocated == 1)
    {
        // case 1: *A(A->F)A*
        set_allocated(req, 0);
        set_allocated(req_footer, 0);
    }   
    else if(next_allocated == 0 && prev_allocated == 1)
    {
        // case 2: *A(A->F)FA
        // ==> *AFFA ==> *A[FF]A merge current and next
        
        // [next]
        // [cur]
        set_allocated(req, 0);
        set_blocksize(req, req_blocksize + next_blocksize);

        set_allocated(next_footer, 0);
        set_blocksize(next_footer, req_blocksize + next_blocksize);
  
    } 
    else if (next_allocated == 1 && prev_allocated == 0)
    {
        // case 3: AF(A->F)A*
        // ==> AFFA* ==> A[FF]A* merge current and prev

        // [cur]
        // [prev]
        set_allocated(prev, 0);
        set_blocksize(prev, req_blocksize + prev_blocksize);

        set_allocated(req_footer, 0);
        set_blocksize(req_footer, req_blocksize + next_blocksize);
    }
    else if (next_allocated == 0 && prev_allocated == 0)
    {
        // case 4: AF(A->F)FA
        // ==> AFFFA ==> A[FFF]A merge current and prev and next
        // the new block like following way:
        /*----------------------*/
        /*      next_footer     */      // [new footer]
        /*----------------------*/
        /*      next->payload   */
        /*      next->header    */
        /*      req->footer     */
        /*      req->payload    */      // [new payload]
        /*      req->header     */
        /*      prev_footer     */
        /*      prev_payload    */
        /*----------------------*/
        /*      prev_header     */      // [new header]
        /*----------------------*/
        set_allocated(prev, 0);
        set_blocksize(prev, req_blocksize + prev_blocksize + next_blocksize);

        set_allocated(next_footer, 0);
        set_blocksize(next_footer, req_blocksize + prev_blocksize + next_blocksize);
    }


    printf("exception for free!\n");
    exit(0);
}















int main()
{
    puts("<< ============ [Test Successfully] ============ >>");
    return 0;
}