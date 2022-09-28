#include "exception_handler.h"

/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_de(){
    printf("Divide_error_handler \n");
    real_halt(256);
}
/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/

void exception_db(){
    printf("Debug_handler \n");
    real_halt(256);
}

/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_nmi(){
    printf("NMI_interrupt_handler \n");
    real_halt(256);
}

/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_bp(){
    printf("Breakpoint_handler \n");
    real_halt(256);
}

/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_of(){

    printf("Overflow_handler \n");
    real_halt(256);
}

/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_br(){
    printf("Bound_range_exceeded_handler \n");
    real_halt(256);
}
/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_ud(){
    printf("Invalid_opcode_handler \n");
    real_halt(256);
}

/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_nm(){
    printf("Device_not_available_handler \n");
    real_halt(256);
}

/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_df(){
    printf("Couble_fault_handler \n");
    real_halt(256);
}
/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_mf(){
    printf("CoProcessor_segment_overrun_handler \n");
    real_halt(256);
}
/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_ts(){
    printf("Invalid_TSS_handler \n");
    real_halt(256);
}
/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_np(){
    printf("Segment_not_present_handler \n");
    real_halt(256);
}
/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_ss(){
    printf("Stack_segmentation_fault_handler \n");
    real_halt(256);
}
/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_gp(){
    printf("General_protection_handler \n");
    real_halt(256);
}
/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_pf(){
    printf("Page_fault_handler \n");
    real_halt(256);
}
/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_fp(){
    printf("Floating_point_error_handler \n");
    real_halt(256);
}
/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_ac(){
    printf("Alignment_check_handler \n");
    real_halt(256);
}
/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_mc(){
    printf("Macine_check_handler \n");
    real_halt(256);
}
/*exception handler
Input:none
output:none
return value:none
effect: stay in handler loop*/
void exception_xf(){
    printf("SIMD_floating_point_exception_handler \n");
    real_halt(256);
}
