#ifndef FILES_H
#define FILES_H

#include "types.h"
#include "lib.h"
#include "rtc.h"
#include "terminal.h"


/* initialize dir_buf to a size of 32  */
char dir_buf[32];

// structure for the function jump table 
typedef struct function_jump_table{
  int32_t (*read)(int32_t fd, void* buf, int32_t nbytes);
  int32_t (*write)(int32_t fd, const void* buf, int32_t nbytes);
  int32_t (*open)(const uint8_t* filename);
  int32_t (*close)(int32_t fd);

} function_jump_table_t;


// structure for file descriptor table
typedef struct file_descriptor{
  function_jump_table_t* file_operation_table_ptr;
  uint32_t inode;
  uint32_t position;
  uint32_t flags;
}fd_t;

// structure for PCB
typedef struct process_control_block  pcb_t;
struct process_control_block{
  fd_t fdt[8];
  int pid;
  int ebp;
  int esp;
  pcb_t* parent;    // ptr to the PCB of parent process
  uint8_t argument[128];
};

/* structure for directory entries  */
typedef struct dentry{
    uint8_t fname[32];  /* file names have size of 32 bytes  */
    uint32_t file_type;
    uint32_t inode_index;
    uint8_t reserved[24];  /* 24 bytes are reserved  */
}dentry_t;

/* structure for boot block  */
typedef struct boot_block{

  uint32_t num_dir_entry;
  uint32_t num_inodes;
  uint32_t num_data_blocks;
  uint8_t reserved[52];  /* 52 bytes are reserved  */
  dentry_t dir_entries[63];  /* the file system can hold up to 63 files  */

}boot_block_t;


/* structure for inode blocks  */
typedef struct inode_block{ //63 in total
  uint32_t length;
  uint32_t data_block_list[1023];  /* an inode can hold up to 1023 data blocks  */
}inode_block_t;

/* structure for data blocks  */
typedef struct data_block{
  uint8_t data_block_starting_addr[4096];   /* size of a data block is 4kB  */
}data_block_t;


boot_block_t* boot_block;


extern void file_init(uint32_t * starting_addr);

extern int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);

extern int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);

extern int32_t file_write(int32_t fd, const void* buf, int32_t nbytes);

extern int32_t file_read(int32_t fd, void* buf, int32_t nbytes);

extern int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

extern int32_t dir_close(int32_t fd);

extern int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes);

extern int32_t dir_open(const uint8_t* fname);

extern int32_t file_close(int32_t fd);

extern int32_t file_open(const uint8_t* fname);

extern int32_t dir_read(int32_t fd, void* buf, int32_t nbytes);

extern function_jump_table_t* find_table_point(uint32_t file_type);

pcb_t* get_cur_pcb(void);

int32_t write_for_in(int32_t fd, const void* buf, int32_t nbytes);

int32_t read_for_out(int32_t fd, void* buf, int32_t nbytes);

#endif
