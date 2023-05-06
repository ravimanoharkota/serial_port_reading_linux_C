
//#include <stddef.h> //for size_t

#include "master_header.h"
#include "ringbuf.h"
//#include "uart_pkt.h"


#define APP_RX_BUFF_SIZE 2000


//shift to ninja_serial_rx.c
#define SERIAL_RX_BUFF_SIZE 2000

#if 0
/*
* External functions
*/
extern void ringbuf_init(void);
extern int ringbuf_enque_multiple(void *databuffer, size_t databuffer_len);
extern int ringbuf_deque_multiple(void *databuffer_out, size_t databuffer_len);
extern void display(void);
#endif

