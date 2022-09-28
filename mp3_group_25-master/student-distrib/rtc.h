/* rtc.h - Defines for RTC
 */
#include "types.h"

extern void rtc_init();    /* Initialize RTC */
extern void rtc_handler();
extern int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t rtc_write(int32_t fd,  const void* buf, int32_t nbytes);
extern int32_t rtc_open(const uint8_t* filename);
extern int32_t rtc_close(int32_t fd);
