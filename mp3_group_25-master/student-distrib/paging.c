
#include "paging.h"
#include "lib.h"

//declear the page_directory
page_directory_t page_dir[1024] __attribute__((aligned (4096)));

//declear the page_table
paging_table_entry_t page_table[1024] __attribute__((aligned (4096)));

//declear the page_table
paging_table_entry_t page_table_VM[1024] __attribute__((aligned (4096)));


/*
 *  paging_init()
 *   DESCRIPTION: initialization of the paging
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: None
 *   SIDE EFFECTS: initialize the paging
 */
void paging_init(){

int index_for_page_dir, index_for_page_table;  // index for loop


//set unused page table to not represent
for (index_for_page_table = 0; index_for_page_table < 1024; index_for_page_table++) {
  page_table[index_for_page_table].present = 0; //NOT present
  page_table[index_for_page_table].R_W = 0;
  page_table[index_for_page_table].U_S = 0;
  page_table[index_for_page_table].PWT = 0;
  page_table[index_for_page_table].PCD = 0;
  page_table[index_for_page_table].A = 0;
  page_table[index_for_page_table].D = 0;
  page_table[index_for_page_table].PAT = 0;
  page_table[index_for_page_table].G = 0;
  page_table[index_for_page_table].AVL = 0;
  page_table[index_for_page_table].Addr_4k = 0;
}

//init the page_table for video memory
 page_table[VIDEO_MEM_ADDR].present = 1;  // video mem is present
 page_table[VIDEO_MEM_ADDR].R_W = 1;
 page_table[VIDEO_MEM_ADDR].U_S = 0;
 page_table[VIDEO_MEM_ADDR].PWT = 0;
 page_table[VIDEO_MEM_ADDR].PCD = 0;
 page_table[VIDEO_MEM_ADDR].A = 0;
 page_table[VIDEO_MEM_ADDR].D = 0;
 page_table[VIDEO_MEM_ADDR].PAT = 0 ;
 page_table[VIDEO_MEM_ADDR].G  = 0 ;
 page_table[VIDEO_MEM_ADDR].AVL = 0;
 page_table[VIDEO_MEM_ADDR].Addr_4k = VIDEO_MEM_ADDR;

 page_table[VIDEO_MEM_ADDR + 1].present = 1;  // video mem is present
 page_table[VIDEO_MEM_ADDR + 1].R_W = 1;
 page_table[VIDEO_MEM_ADDR + 1].U_S = 0;
 page_table[VIDEO_MEM_ADDR + 1].PWT = 0;
 page_table[VIDEO_MEM_ADDR + 1].PCD = 0;
 page_table[VIDEO_MEM_ADDR + 1].A = 0;
 page_table[VIDEO_MEM_ADDR + 1].D = 0;
 page_table[VIDEO_MEM_ADDR + 1].PAT = 0 ;
 page_table[VIDEO_MEM_ADDR + 1].G  = 0 ;
 page_table[VIDEO_MEM_ADDR + 1].AVL = 0;
 page_table[VIDEO_MEM_ADDR + 1].Addr_4k = VIDEO_MEM_ADDR + 1;

 page_table[VIDEO_MEM_ADDR + 2].present = 1;  // video mem is present
 page_table[VIDEO_MEM_ADDR + 2].R_W = 1;
 page_table[VIDEO_MEM_ADDR + 2].U_S = 0;
 page_table[VIDEO_MEM_ADDR + 2].PWT = 0;
 page_table[VIDEO_MEM_ADDR + 2].PCD = 0;
 page_table[VIDEO_MEM_ADDR + 2].A = 0;
 page_table[VIDEO_MEM_ADDR + 2].D = 0;
 page_table[VIDEO_MEM_ADDR + 2].PAT = 0 ;
 page_table[VIDEO_MEM_ADDR + 2].G  = 0 ;
 page_table[VIDEO_MEM_ADDR + 2].AVL = 0;
 page_table[VIDEO_MEM_ADDR + 2].Addr_4k = VIDEO_MEM_ADDR + 2;

 page_table[VIDEO_MEM_ADDR + 3].present = 1;  // video mem is present
 page_table[VIDEO_MEM_ADDR + 3].R_W = 1;
 page_table[VIDEO_MEM_ADDR + 3].U_S = 0;
 page_table[VIDEO_MEM_ADDR + 3].PWT = 0;
 page_table[VIDEO_MEM_ADDR + 3].PCD = 0;
 page_table[VIDEO_MEM_ADDR + 3].A = 0;
 page_table[VIDEO_MEM_ADDR + 3].D = 0;
 page_table[VIDEO_MEM_ADDR + 3].PAT = 0 ;
 page_table[VIDEO_MEM_ADDR + 3].G  = 0 ;
 page_table[VIDEO_MEM_ADDR + 3].AVL = 0;
 page_table[VIDEO_MEM_ADDR + 3].Addr_4k = VIDEO_MEM_ADDR + 3;

//init of 4kb page
page_dir[0].present = 1; //4kb is present
page_dir[0].R_W = 1;
page_dir[0].U_S = 0;
page_dir[0].PWT = 0;
page_dir[0].PCD = 0;
page_dir[0].A = 0;
page_dir[0].Ignored = 0;
page_dir[0].Size = 0;  // 0 means it is a 4K page directory entry
page_dir[0].G = 0;
page_dir[0].AVL = 0;
page_dir[0].Addr31_12 = (unsigned int) page_table >> 12; //addr of page table

//initialization of 4MB page

page_dir[1].present = 1; //kernel is present
page_dir[1].R_W = 1;
page_dir[1].U_S = 0;
page_dir[1].PWT = 0;
page_dir[1].PCD = 0;
page_dir[1].A = 0;
page_dir[1].Ignored = 0;
page_dir[1].Size = 1;   //  1 means it is a 4M page directory entry
page_dir[1].G = 1;
page_dir[1].AVL = 0;
page_dir[1].Addr31_12 = 0x00400;  // this is the 4MB addr

//set rest 8MB -> 4GB to not present
for (index_for_page_dir = 2; index_for_page_dir < 1024; index_for_page_dir++) {

  page_dir[index_for_page_dir].present = 0;
  page_dir[index_for_page_dir].R_W = 0;
  page_dir[index_for_page_dir].U_S = 0;
  page_dir[index_for_page_dir].PWT = 0;
  page_dir[index_for_page_dir].PCD = 0;
  page_dir[index_for_page_dir].A = 0;
  page_dir[index_for_page_dir].Ignored = 0;
  page_dir[index_for_page_dir].Size = 1;   //  1 means it is a 4M page directory entry
  page_dir[index_for_page_dir].G = 0;
  page_dir[index_for_page_dir].AVL = 0;
  page_dir[index_for_page_dir].Addr31_12 = 0;

}

  int addr_132M = 33; // 132MB/4MB   0x8400000
  page_dir[addr_132M].present = 0; //program image page is present
  page_dir[addr_132M].R_W = 1;
  page_dir[addr_132M].U_S = 1;
  page_dir[addr_132M].PWT = 0;
  page_dir[addr_132M].PCD = 0;
  page_dir[addr_132M].A = 0;
  page_dir[addr_132M].Ignored = 0;
  page_dir[addr_132M].Size = 0;   //  0 means it is a page table entry
  page_dir[addr_132M].G = 0;
  page_dir[addr_132M].AVL = 0;
  page_dir[addr_132M].Addr31_12 =(unsigned int)  page_table_VM >> 12;  //addr of page table


//rest entry in kernel is not present
for (index_for_page_table = 0; index_for_page_table < 1024; index_for_page_table++) {
    page_table_VM[index_for_page_table].present = 0; //NOT present
    page_table_VM[index_for_page_table].R_W = 0;
    page_table_VM[index_for_page_table].U_S = 0;
    page_table_VM[index_for_page_table].PWT = 0;
    page_table_VM[index_for_page_table].PCD = 0;
    page_table_VM[index_for_page_table].A = 0;
    page_table_VM[index_for_page_table].D = 0;
    page_table_VM[index_for_page_table].PAT = 0;
    page_table_VM[index_for_page_table].G = 1;
    page_table_VM[index_for_page_table].AVL = 0;
    page_table_VM[index_for_page_table].Addr_4k = 0;
}

// unsigned int pointer = (&page_directory) & 0xfffff000;  //top 20 bit for page_dir
loadPageDirectory( (unsigned int*)page_dir);
enablePaging();

}


/*
 *   flush_tlb()
 *   DESCRIPTION: flush tlb by assign new page table
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: None
 */
void flush_tlb(){

  asm volatile (
    //flush tlb
    " movl	%%cr3,%%eax    ;"
    " movl	%%eax,%%cr3   ;"
      :
      :
      : "eax" , "cc"
  );

}

/*
 *   paging_set_up()
 *   DESCRIPTION: set up program image
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: None
 *   SIDE EFFECTS: map the program image to physical memory
 */
void paging_set_up(int process_num) {
  int addr_128M = 32; // 128MB/4MB
  page_dir[addr_128M].present = 1; //program image page is present
  page_dir[addr_128M].R_W = 1;
  page_dir[addr_128M].U_S = 1;
  page_dir[addr_128M].PWT = 0;
  page_dir[addr_128M].PCD = 0;
  page_dir[addr_128M].A = 0;
  page_dir[addr_128M].Ignored = 0;
  page_dir[addr_128M].Size = 1;   //  1 means it is a 4M page directory entry
  page_dir[addr_128M].G = 1;
  page_dir[addr_128M].AVL = 0;
  page_dir[addr_128M].Addr31_12 = (0x00800 + 0x00400*process_num); //this is 8MB addr plus 4Kb*pid
  flush_tlb();
}


/*
 *   vidmap_paging_set_up
 *   DESCRIPTION: set up viedeo page
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: None
 *   SIDE EFFECTS: map the new virtual video page to physical video memory
 */
int32_t vidmap_paging_set_up(){

  int addr_132M = 33; // 132MB/4MB   0x8400000
  page_dir[addr_132M].present = 1; //program image page is present


//rest entry in kernel is not present

  // map first page table entry to video mem in kernel
  page_table_VM[VIDEO_MEM_ADDR].present = 1; // present
  page_table_VM[VIDEO_MEM_ADDR].R_W = 1;
  page_table_VM[VIDEO_MEM_ADDR].U_S = 1;
  page_table_VM[VIDEO_MEM_ADDR].Addr_4k = VIDEO_MEM_ADDR;
  flush_tlb();
  return VIDEO_MEM_USR_ADDR;//
}

