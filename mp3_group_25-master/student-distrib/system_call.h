#ifndef SYSTEM_CALL_H
#define SYSTEM_CALL_H

#include "types.h"
#include "files.h"
#include "paging.h"
#include "x86_desc.h"
#include "lib.h"

extern int32_t halt(uint8_t status);

extern int32_t execute(const uint8_t* command);

extern int32_t read (int32_t fd, void* buf, int32_t nbytes);

extern int32_t write (int32_t fd, const void* buf, int32_t nbytes);

extern int32_t open (const uint8_t* filename);

extern int32_t close (int32_t fd);

extern void set_up_io_file(pcb_t* pcb);

extern pcb_t* get_cur_pcb(void);

extern int32_t getargs(uint8_t* buf, int32_t nbytes);

extern int32_t vidmap (uint8_t** screen_start);

extern int32_t real_halt (uint32_t status);

#endif

