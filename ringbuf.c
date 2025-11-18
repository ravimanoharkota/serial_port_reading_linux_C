
//https://gist.githubusercontent.com/Kinjalrk2k/5c6feeecc5dccb2fb43027a711358e77/raw/1a5c55a393a10712e9c0e84b823c97c193313bf2/ring_buffer.c

/*
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
*/

#include "ringbuf.h"

/*
typedef struct __uart_pkt_buf
{
char uart_pkt_data[201];
int len;
int pktid;
}_uart_pkt_buf;

typedef struct Node{
    //int data;
    void *data;
    int empty;
    struct Node* next;
}node;  //  nickname :)

typedef struct _ring_buffer{
    node *buff_start;
    node *start;
    node *end;
};
*/

//extern _ring_buffer b={0};

//------------PKT FUNCTIONS------
void print_uart_pkt(_uart_pkt_buf *pkt_data) 
{
  
/*
typedef struct __uart_pkt_buf
{
char uart_pkt_data[200];
int len;
int pktid;
}_uart_pkt_buf;

*/
int len=0;
_uart_pkt_buf *uart_pkt_buf_ptr = (_uart_pkt_buf *)pkt_data;
len = uart_pkt_buf_ptr->len;
printf("\n\n_______________________________________________________________________\n");
printf("len:%d pktid:%d\n",uart_pkt_buf_ptr->len, uart_pkt_buf_ptr->pktid); 
printf("_______________________________________________________________________\n");
for(int i=0; i<len; i++)
{
  printf("%c", uart_pkt_buf_ptr->uart_pkt_data[i]);
}
printf("_______________________________________________________________________\n\n");
//printf("%s", uart_pkt_buf_ptr->uart_pkt_data);
//printf("---------------------------------");
}

void print_address(void)
{
  printf("b.start->data@%p, b.end->data@%p b.buff_start->data@%p",
         b.start->data,
         b.end->data,
         b.buff_start->data);

}
//RING BUFFER FUNCTIONS--------------------
void ringbuf_init(void)
{
   //_uart_pkt_buf uart_pkt_buf1={0};
   int size = RING_BUFFER_SIZE; //use default size of 10 slots

   b.start = NULL;
   b.end = NULL;
   b.buff_start = NULL;
  

    b.start = (node *) malloc(sizeof(node));
    b.start->data = (_uart_pkt_buf *)malloc(sizeof(_uart_pkt_buf));

    b.start->empty = 1;
    b.start->next = NULL;

    node *ptr = b.start;
    for(int i=0; i<size-1; i++){
        node *newNode = (node *) malloc(sizeof(node));
        newNode->empty = 1;
        newNode->next = NULL;
        newNode->data = (_uart_pkt_buf *)malloc(sizeof(_uart_pkt_buf));

        //check
        if(ptr->data == NULL)
        {
          printf("ptr->data is NULL while creating rb at i:%d", i);
          exit(-1);
        }
  
       //link up prev allocated node to this newNode memblock 
       ptr->next = newNode;
        ptr = ptr->next; //advance pointer to this newNode as prep for next node's linkup
    }//for loop ends

    //Fix up end node's next pointing to b.start
    ptr->next = b.start;

    //Set all pointers to the "finish line" flagstaff
    b.end = b.start;
    b.buff_start = b.start;
 
    if(b.end == NULL)
    {
     printf("b.end NULL after creating rb");
     exit(-1);
    }
    if(!b.start || !b.buff_start)
    {
      printf("b.strat or bff start NULL after creating rb");
     exit(-1);

    }
    if(b.end->data == NULL)
    {
     printf("b.end->data is NULL after creating rb");
     exit(-1);
    }

    printf("rb init done");
    print_address();
}

void display(void)
{
    node* ptr = b.buff_start;
    _uart_pkt_buf *pkt_data;
    do{
        if(ptr->empty == 1)
            printf("ZZ-> ");
        else
            if( !ptr || !(ptr->data)) {
                printf("display(): ptr or ptr->data NULL, exit");
                exit(-1);
            }
            pkt_data =  (_uart_pkt_buf *)ptr->data;
            print_uart_pkt(pkt_data);
        ptr = ptr->next;
    }while(ptr != b.buff_start);
    printf("\b\b\b\b   \b\b\b\n");
    print_address();
}

/* expects the caller to allocate pkt_data heap memory, enqueue and free it in caller
 *  afterwards.
 */
int enqueue(void *pkt_data_in)
{
  _uart_pkt_buf *pkt_data = (_uart_pkt_buf *)pkt_data_in;

printf("before enque:\n\n");
    print_address();
if(b.end == NULL)
{
     printf("b.end NULL exit");
     return RING_BUFFER_ENQUEUE_FAIL;
}
if(b.end->data == NULL) {
   printf("b.end->data is NULL");
   return RING_BUFFER_ENQUEUE_FAIL;
}  

#if 0
//dont enqueue to an already enqueued slot
if(b.end->empty == 0)
{
    printf("enqueue(): ERROR - cant enqueue to already enqueued slot !!\n");
    return RING_BUFFER_ENQUEUE_FAIL;
}
#endif

   // printf("b.end->data available....");

  if(pkt_data_in == NULL) {
    printf("pkt_data_in NULL!");
    return RING_BUFFER_ENQUEUE_FAIL;
  }
    //Enque to the end slot, pointed to by the 'end' pointer
    //memcpy(b.end->data, (_uart_pkt_buf *)pkt_data_in, (sizeof(_uart_pkt_buf)-1));

#if 0
    /*Some pkt parsing before actual enqueue */
    if(pkt_data_in->len==0) {
     printf("malformed packet, drop instead of enqueue");
     return b;
    }
#endif

    printf("pkt good, about to memcpy\n\n");
    memcpy(b.end->data, pkt_data, sizeof(_uart_pkt_buf));

  printf("enqueued pktid:%d of len:%d bytes\n\n", pkt_data->pktid, pkt_data->len);
    print_address();
    //b.end->data = pkt_data;
    b.end->empty = 0;
    //advance pointer to next end slot as  preparation for then next fill up.
    //not mallocating() the new node memory, just move to preallocated next slot
    b.end = b.end->next;

  //printf("enqueued pktid:%d of len:%d bytes\n", pkt_data->pktid, pkt_data->len);

    return RING_BUFFER_ENQUEUE_SUCCESS;

}

/* expects the caller to allocate pkt_data heap memory, this dequeues into it, 
 *  and free it in caller afterwards.
 */
int dequeue(void *pkt_data_out)
{
    //void *pkt_data_out = b.start->data;
    _uart_pkt_buf *pkt_data = (_uart_pkt_buf *)pkt_data_out;
 printf("before dewq"); 
    print_address();
if(b.start == NULL)
{//This should never happen
     printf("deque(): b.start NULL exit\n");
     return RING_BUFFER_DEQUEUE_FAIL;
}

   if (b.start->data == NULL) {
     printf("deque(): Cant Dequeu, start data null\n");
   }

#if 0
//dont dequeue from an already emptied (dequeued) slot
if((b.start->empty == 1))
{
    printf("deque(): ERROR - cant dequeue from already emptied slot !!\n");
    return RING_BUFFER_DEQUEUE_FAIL;
}
#endif

    //Dequeue from the start slot, mark as emptied and advance pointer to next slot
    memcpy((_uart_pkt_buf *)pkt_data_out, b.start->data, sizeof(_uart_pkt_buf));
    b.start->empty = 1;
    //advance the 'start' pointer to next filled slot as preparation for next dequeue.
    //not freeing() up the slot's malloc'ed memory, just move to preallocated next slot
    b.start = b.start->next;

    if( pkt_data && (pkt_data->len > 0)) {
      printf("dequeued pktid:%d of len:%d bytes\n", pkt_data->pktid, pkt_data->len);
      //print_uart_pkt(pkt_data);
    }
    else
    {
      
      printf("invalid pkt got\n");
      if(!pkt_data ) printf("pkt_data NULL");
      if(pkt_data->len <= 0)printf("pktlen < 0");
    }

 printf("after dewq"); 
    print_address();
    return RING_BUFFER_DEQUEUE_SUCCESS;
}//deque()

/* Continuos enqueue of multiple packets each of fixed length */
int ringbuf_enque_multiple(void *databuffer, size_t databuffer_len)
{
 char *ptr = databuffer;
 int i=0, pkt_count, res=RING_BUFFER_ERROR;
 if(ptr==NULL)
 {
   printf("Error: ringbuf_enque_multiple(): empty payload !!");
   return RING_BUFFER_ERROR; 
 }
 
   //Calculate no of packets that can be can be chopped
   //from the given linear buffer
   pkt_count = (databuffer_len % sizeof(_uart_pkt_buf));

    //Enqueue pkt_count number of uart packets 
    //for(int i=0; i<pkt_count; i++)

    for(int i=0; i<5; i++)
    {
      //trigger the enqueue
      if( (res = enqueue(ptr)) < 0) {
       //printf("ringbuf_enque_multiple() - ENQUE ERROR");
      }
      ptr = ptr + sizeof(_uart_pkt_buf);
    }
  printf("rb enque multiple success");
  return RING_BUFFER_SUCCESS;
}


/* Continuos dequeue of multiple packets each of fixed length */
int ringbuf_deque_multiple(void *databuffer_out, size_t databuffer_len)
{
 char *ptr = (char *)databuffer_out;
 _uart_pkt_buf uart_pkt_buf2={0};
 int pkt_count, res=RING_BUFFER_ERROR;

 if(ptr == NULL)
 {
   printf("ringbuf_deque_multiple(): databuffer_out is NULL !!");
   return RING_BUFFER_ERROR;
 }

 int i=0;
 if(ptr==NULL)
 {
   printf("Error: ringbuf_enque_multiple(): empty payload !!");
   return RING_BUFFER_ERROR; 
 }
 
   //Calculate no of packets that can fit in the given linear buffer
   pkt_count = (databuffer_len % sizeof(_uart_pkt_buf));

    //Dequeue pkt_count number of uart packets 
    for(int i=0; i<pkt_count; i++)
    {
      // ready packet buffer to get dequed content
      memset(&uart_pkt_buf2, '\0', sizeof(_uart_pkt_buf));

      //call the dequeue
      if( (res = dequeue(&uart_pkt_buf2)) < 0) {
       printf("DEQUE ERROR");
      }
      memcpy(ptr, &uart_pkt_buf2, sizeof(_uart_pkt_buf));
      ptr=ptr+sizeof(_uart_pkt_buf);
     
       
    }

    ptr[databuffer_len] = '\0';

  printf("rb deque multiple success");
    return RING_BUFFER_SUCCESS;
}

#if 0 //prbably crashing failing func
//simulation - TODO replace by real serial.read()
void build_packet_to_simulate_enque(char *buf_in, size_t size)
{
     char *ptr = buf_in;
     _uart_pkt_buf uart_pkt={0};
     int pkt_count=0;
     if(!ptr)
     {
       printf("ERROR build_packet_to_simulate_enque(): buf_in is NULL");
       return;
     }

   //Calculate no of packets that can fit in the given linear buffer
   pkt_count = (size / sizeof(_uart_pkt_buf));
   
       memset(buf_in, '\0', size);

    //fill in pkt_count number of uart packets 
    for(int i=0; i< pkt_count; i++)
    {
      memset(&uart_pkt, '\0', sizeof(_uart_pkt_buf));
      uart_pkt.len = 50;
      uart_pkt.pktid = i;
      sprintf(uart_pkt.uart_pkt_data, "packet:%d greets you - Hello user", uart_pkt.pktid);
      uart_pkt.uart_pkt_data[50] = '\0';

   
       memcpy(ptr, (void *)&uart_pkt, sizeof(_uart_pkt_buf));
       ptr = ptr + sizeof(_uart_pkt_buf);
    }
    ptr[size-1]='\0';
}//build_packet_to_simulate_enque()
#endif

void build_packet_to_simulate_enque(char *buf_in, size_t size)
{

    //char buf_in[2000];
    //int size= sizeof(buf_in);
     char *ptr = buf_in;
     _uart_pkt_buf uart_pkt={0};
     int pkt_count=0;
     if(!ptr)
     {
       printf("ERROR build_packet_to_simulate_enque(): buf_in is NULL");
       return;
     }

   //Calculate no of packets that can fit in the given linear buffer
   pkt_count = (size / sizeof(_uart_pkt_buf));
  //  printf("pkt_count:%d", pkt_count );
   
       memset(buf_in, '\0', size);

    //fill in pkt_count number of uart packets 
    for(int i=0; i< pkt_count; i++)
    {
      memset(&uart_pkt, '\0', sizeof(_uart_pkt_buf));
      uart_pkt.len = 50;
      uart_pkt.pktid = i;
      //printf("i:%d packet:%d greets you - Hello user\n", i, uart_pkt.pktid);
  
      sprintf(uart_pkt.uart_pkt_data, "packet:%d greets you - Hello user\n", uart_pkt.pktid);
      //printf("%s", uart_pkt.uart_pkt_data);
      //uart_pkt.uart_pkt_data[50] = '\0';

   
       memcpy(ptr, (void *)&uart_pkt, sizeof(_uart_pkt_buf));
       ptr = ptr + sizeof(_uart_pkt_buf);
        
        //printf("bufferend:%u ptr%u", &buf_in[size-1], ptr);
    }
    ptr[size-1]='\0';
}



/*
void main()
{
}
*/

/*
OUTPUT


START is towards the left, and END pointer is on right extreme side.
ENQUEUE from END(right), DEQUEUE from START(left)

ravi@hpPavilion:~/rtes$ 
ravi@hpPavilion:~/rtes$ ./bufring 13
Enter the fixed size of the buffer: 13
1 -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL    
1 -> 2 -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL    
1 -> 2 -> 3 -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL    
1 -> 2 -> 3 -> 4 -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL    
1 -> 2 -> 3 -> 4 -> 5 -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL    
1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL    
1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> NULL -> NULL -> NULL -> NULL -> NULL -> NULL    
1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> NULL -> NULL -> NULL -> NULL -> NULL    
1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> NULL -> NULL -> NULL -> NULL    
1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> NULL -> NULL -> NULL    
1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> 11 -> NULL -> NULL    
DEQUING ------ 
Dequeued:1
NULL -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> 11 -> NULL -> NULL    
Dequeued:2
NULL -> NULL -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> 11 -> NULL -> NULL    
Dequeued:3
NULL -> NULL -> NULL -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> 11 -> NULL -> NULL    
again enque 12 and 13 data 
NULL -> NULL -> NULL -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> 11 -> 12 -> NULL    
NULL -> NULL -> NULL -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> 11 -> 12 -> 13    
ravi@hpPavilion:~/rtes$ 
ravi@hpPavilion:~/rtes$ 


*/
