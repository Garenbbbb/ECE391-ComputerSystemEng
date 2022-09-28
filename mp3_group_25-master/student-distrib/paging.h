#ifndef PAGING_H
#define PAGING_H

#define VIDEO_MEM_ADDR 0xb8
#define VIDEO_MEM_USR_ADDR 0x8400000 + 0xb8000
#include "types.h"

/*set up program image*/
extern void paging_set_up(int process_num);

/*set up video paging*/
extern int32_t vidmap_paging_set_up();

/*init the paging*/
extern void paging_init();

/*load the page dir to memory*/
extern void loadPageDirectory(unsigned int* );

/*enable the paging by sending bits to cr register*/
extern void enablePaging();


//struct of page table
typedef struct paging_table_entry paging_table_entry_t;
struct paging_table_entry {
    uint32_t present  :1;
    uint32_t R_W      :1;
    uint32_t U_S      :1;
    uint32_t PWT      :1;
    uint32_t PCD      :1;
    uint32_t A        :1;
    uint32_t D        :1;
    uint32_t PAT      :1;
    uint32_t G        :1;
    uint32_t AVL      :3;
    uint32_t Addr_4k  :20;
} __attribute__ ((packed));


/* structure for page-directory entry. We only need to have one struct for both 4kb
  and 4mb since for bit 12 to 21 they both have zeros, and for other bits they basically
  use to hold the same filed.
*/
typedef struct page_directory page_directory_t;
struct page_directory {
    uint32_t present  :1;
    uint32_t R_W      :1;
    uint32_t U_S      :1;
    uint32_t PWT      :1;
    uint32_t PCD      :1;
    uint32_t A        :1;
    uint32_t Ignored  :1;
    uint32_t Size     :1;  //whether 4MB or 4KB
    uint32_t G        :1;
    uint32_t AVL      :3;
    uint32_t Addr31_12   :20;
} __attribute__ ((packed));

#endif

