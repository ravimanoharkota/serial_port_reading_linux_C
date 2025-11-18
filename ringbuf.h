
//#include <stddef.h>
//#include <stdio.h>
//#include <malloc.h>
//#include <string.h>
//#include <stdlib.h>

#include "master_header.h"
#include "uart_pkt.h"

#define RING_BUFFER_SIZE 10

#define RING_BUFFER_ENQUEUE_SUCCESS 0
#define RING_BUFFER_ENQUEUE_FAIL -1
#define RING_BUFFER_DEQUEUE_SUCCESS 0
#define RING_BUFFER_DEQUEUE_FAIL -1

#define RING_BUFFER_ERROR -1
#define RING_BUFFER_SUCCESS 0

typedef struct Node{ 
    //int data; 
    void *data; 
    int empty; 
    struct Node* next; 
}node;  //  nickname :) 

typedef struct ring_buffer{
    node *buff_start;
    node *start;
    node *end;
}_ring_buffer;

static _ring_buffer b={0};

/*
* External functions
*/
extern void ringbuf_init(void);
extern int ringbuf_enque_multiple(void *databuffer, size_t databuffer_len);
extern int ringbuf_deque_multiple(void *databuffer_out, size_t databuffer_len);
extern void display(void);
extern void build_packet_to_simulate_enque(char *buf_in, size_t size);
extern void print_address(void);


extern int enqueue(void *pkt_data_in);
