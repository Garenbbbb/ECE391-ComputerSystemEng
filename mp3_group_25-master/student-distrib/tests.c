#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "idt.h"
#include "paging.h"
#include "files.h"
#include "types.h"
#include "rtc.h"
#include "terminal.h"

#define PASS 1
#define FAIL 0
#define PASSQ 0x1
#define FD 0
#define FILENAME 1

int32_t rtc_buf[1];

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__);
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

/* IDT Test - Example
 * 
 * Asserts that DE exception works
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, DE exception
 * Files: x86_desc.h/S
 */
void idt_test_1(){
	TEST_HEADER;
	int j = 0;
	int i = 1;
	i = i/j;

	// return FAIL;
}

/* null_pointer_test
 * 
 * Asserts page fault
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: paging
 * Files: paging.h
 */

int null_pointer_test(){
	TEST_HEADER;

	int* pointer = NULL;
	int data;
	data = *(pointer);
	return PASS;
}


/* non_access_mem
 * 
 * Asserts page fault
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: paging
 * Files: paging.h
 */

int non_access_mem(){
	TEST_HEADER;

	int* pointer = (int*)(0x900000);
	int data;
	data = *(pointer);
	return PASS;
}

/*video_mem_test()
 * 
 * Asserts pass
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: paging
 * Files: paging.h
 */

int video_mem_test(){
	TEST_HEADER;

	int * pointer = (int*)(0xB8000);
	int data;
	data = *(pointer);
	return PASS;
}


/* kernel_test()
 * 
 * Asserts pass
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: paging
 * Files: paging.h
 */

int kernel_test(){
	TEST_HEADER;

	int * pointer = (int*)(0x400000);
	int data;
	data = *(pointer);
	return PASS;
}

/* read frame 1 test()
 * 
 * Asserts pass
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: file system
 * Files: file.c/frame1.txt
 */


// add more tests here

/* Checkpoint 2 tests */

/* read frame 0 test()
 * 
 * Asserts pass
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: file system
 * Files: file.c/frame0.txt
 */
// int read_frame_test0() {
// 	TEST_HEADER;

// 	uint32_t res;
   
// 	clear();  // clear the screen
// 	file_open((uint8_t*)"frame0.txt");
// 	char buf[200];     // read 200 bytes
// 	res = file_read(0,buf,200);

// 	int i;    // loop index
// 	// printf("%d",res);
// 	for(i = 0; i < res; i++) putc(buf[i]);

// 	file_close(0);    // 0 is the fd value
// 	return PASS;
// }

// /* read frame 1 test()
//  * 
//  * Asserts pass
//  * Inputs: None
//  * Outputs: PASS/FAIL
//  * Side Effects: None
//  * Coverage: file system
//  * Files: file.c/frame1.txt
//  */
// int read_frame_test1() {
// 	TEST_HEADER;

// 	uint32_t res;

// 	clear();  // clear the screen
// 	file_open((uint8_t*)"frame1.txt");
// 	char buf[200];    // read 200 bytes
// 	res = file_read(0,buf,200);
// 	int i;
// 	// printf("%d",res);
// 	for(i = 0; i < res; i++) putc(buf[i]);
// 	return PASS;
// }

// /* read file with long name and long data test()
//  * 
//  * Asserts pass
//  * Inputs: None
//  * Outputs: PASS/FAIL
//  * Side Effects: None
//  * Coverage: file system
//  * Files: file.c/frame0.txt
//  */
// int read_longname_file() {
// 	TEST_HEADER;
//     // dentry_t fi;
// 	clear();

// 	uint32_t res;
//     // read_dentry_by_name("frame0.txt", &fi);
// 	file_open((uint8_t*)"verylargetextwithverylongname.txt");
// 	char buf[6000];
// 	// res = read_data(fi.inode_index, 0, buf, 200);
// 	res = file_read(0,buf,6000);
// 	int i;
// 	// printf("%d",res);
// 	for(i = 0; i < res; i++) {
// 		if (buf[i] != NULL) {
// 			putc(buf[i]);
// 		}
// 	}
// 	return PASS;
// }

// /* read executable file()
//  * 
//  * Asserts pass
//  * Inputs: None
//  * Outputs: PASS/FAIL
//  * Side Effects: None
//  * Coverage: file system
//  * Files: file.c/ls
//  */
// int read_ls_file() {
// 	TEST_HEADER;
//     // dentry_t fi;
// 	clear();

// 	uint32_t res;
//     // read_dentry_by_name("frame0.txt", &fi);
// 	file_open((uint8_t*)"ls");
// 	char buf[6000];
// 	// res = read_data(fi.inode_index, 0, buf, 200);
// 	res = file_read(0,buf,6000);
// 	int i;
// 	// printf("%d",res);
// 	for(i = 0; i < res; i++) {
// 		if (buf[i] != NULL) {
// 			putc(buf[i]);
// 		}
// 	}
// 	return PASS;
// }

// /* read diretory test()
//  * 
//  * Asserts pass
//  * Inputs: None
//  * Outputs: PASS/FAIL
//  * Side Effects: None
//  * Coverage: file system
//  * Files: file.c
//  */
// int dir_read_test() {
// 	TEST_HEADER;

// 	clear();  // clear screen

// 	dir_open((uint8_t*)".");
// 	int i, j;
// 	char buf[32];
// 	for(i = 0; i < boot_block->num_dir_entry; i++) {
// 		dir_read(0,buf,32);
// 		printf("file = ");
// 		for(j = 0;j < 32;j ++){
// 			putc(buf[j]);
// 		}
// 		printf("\n");
// 	}
// 	return PASS;
// }


// /* read non-existed file
//  * 
//  * Asserts pass
//  * Inputs: None
//  * Outputs: PASS/FAIL
//  * Side Effects: None
//  * Coverage: file system
//  * Files: file.c/ls
//  */
// int read_non_existed_file() {
// 	TEST_HEADER;
   
//     // If return value is -1, then it is correct.
// 	if (-1 == file_open((uint8_t*)"nothisfile.txt")) {
// 		return PASS;
// 	}
//         return FAIL;
// }

// /* read non-existed directory
//  * 
//  * Asserts pass
//  * Inputs: None
//  * Outputs: PASS/FAIL
//  * Side Effects: None
//  * Coverage: file system
//  * Files: file.c/ls
//  */
// int read_non_existed_dir() {
// 	TEST_HEADER;
   
//     // If return value is -1, then it is correct.
// 	if (-1 == dir_open((uint8_t*)"nothisdir")) {
// 		return PASS;
// 	}
//         return FAIL;
// }


// /* rtc_test()
//  * 
//  * Asserts pass
//  * Inputs: None
//  * Outputs: PASS/FAIL
//  * Side Effects: None
//  * Coverage: rtc
//  * Files: rtc.h
//  */
// int rtc_test(){
//     TEST_HEADER;

//     int i;  /* loop index */

//     rtc_open((void* )FILENAME);
    
// 	 /* print "1" at freq of 2Hz 10 times */
//     for(i = 0; i < 10; i++) {
//         if (rtc_read(FD, NULL, 0) == 0) {
//             printf("%d", 1);
//         }
//     }
    
// 	 /* print "2" at freq of 4Hz 20 times */
//     rtc_buf[0] = 4;
//     rtc_write(FD, rtc_buf, 0);
//     for(i = 0; i < 20; i++) {
//         if (rtc_read(FD, NULL, 0) == 0) {
//             printf("%d", 2);
//         }
//     }
    
// 	/* print "3" at freq of 16Hz 40 times */
//     rtc_buf[0] = 16;
//     rtc_write(FD, rtc_buf, 0);
//     for(i = 0; i < 40; i++) {
//         if (rtc_read(FD, NULL, 0) == 0) {
//             printf("%d", 3);
//         }
//     }
    
// 	/* print "4" at freq of 128Hz 80 times */
//     rtc_buf[0] = 128;
//     rtc_write(FD, rtc_buf, 0);
//     for(i = 0; i < 80; i++) {
//         if (rtc_read(FD, NULL, 0) == 0) {
//             printf("%d", 4);
//         }
//     }
    
// 	/* print "5" at freq of 512Hz 160 times */
//     rtc_buf[0] = 512;
//     rtc_write(FD, rtc_buf, 0);
//     for(i = 0; i < 160; i++) {
//         if (rtc_read(FD, NULL, 0) == 0) {
//             printf("%d", 5);
//         }
//     }
    
// 	/* print "6" at freq of 1024Hz 800 times */
//     rtc_buf[0] = 1024;
//     rtc_write(FD, rtc_buf, 0);
//     for(i = 0; i < 800; i++) {
//         if (rtc_read(FD, NULL, 0) == 0) {
//             printf("%d", 6);
//         }
//     }

// 	rtc_close(FD);

//     return PASS;
// }

// /* rtc_nonexisted_freq_test()
//  * 
//  * Asserts pass
//  * Inputs: None
//  * Outputs: PASS/FAIL
//  * Side Effects: None
//  * Coverage: rtc
//  * Files: rtc.h
//  */
// int rtc_nonexisted_freq_test() {
// 	TEST_HEADER;
   
//     rtc_open((void* )FILENAME);
    
// 	// change freq to a number that is not a power of 2
//     rtc_buf[0] = 73;
//     rtc_write(FD, rtc_buf, 0);


//     // If return value is -1, then it is correct.
// 	if (-1 == rtc_write(FD, rtc_buf, 0)) {
// 		return PASS;
// 	}
//         return FAIL;
// }

// /* terminal_test()
//  * 
//  * Asserts pass
//  * Inputs: None
//  * Outputs: PASS/FAIL
//  * Side Effects: None
//  * Coverage: rtc
//  * Files: terminal.h
//  */
// void terminal_test() {
// 	clear();
//     char buf[128];
//     // char buf_write[6] = "123456";
//     int32_t num_read;

//     // test 0 for terminal write
//     // terminal_write(1, buf_write, 6);

//     // test 1
//     // num_read = terminal_read(1, buf, 128);
//     // terminal_write(1, buf, num_read);
    
//     // test 2
//     while(1)
//     {   
//         //128 is max number of buffer size
//         //1 for fd filename
//         num_read = terminal_read(1, buf, 128);
//         terminal_write(1, buf, num_read);
//     }

// }

/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// TEST_OUTPUT("idt_test", idt_test());

	// launch your tests here
	// idt_test_1();
	// TEST_OUTPUT("idt_test", page_test_1());
	// TEST_OUTPUT("idt_test", null_pointer_test());
	// TEST_OUTPUT("idt_test", non_access_mem());
	// TEST_OUTPUT("idt_test", video_mem_test());
	// TEST_OUTPUT("file_test", read_frame_test0());
	// TEST_OUTPUT("file_test", read_frame_test1());
	// read_longname_file();
	// TEST_OUTPUT("file_test", read_ls_file());
	// TEST_OUTPUT("file_test", dir_read_test());
	// TEST_OUTPUT("file_test", read_non_existed_file());
	// TEST_OUTPUT("file_test", read_non_existed_dir());
	// TEST_OUTPUT("rtc_test", rtc_test());
    // TEST_OUTPUT("rtc_test", rtc_nonexisted_freq_test());
	
}

