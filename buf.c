//gcc 7.4.0

#include  <stdio.h>
#include <stdlib.h>
#include <string.h>

char appbuf[3000];

typedef struct __uart_pkt_buf
{
char uart_pkt_data[100];
int len;
int pktid;
}_uart_pkt_buf;


//simulation - TODO replace by real serial.read()
void simul(char *buf_in, int size)
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
    
    //printf("\n\nbuf:\n%s\n", buf_in);
    
    
}//build_packet_to_simulate_enque()


void main()
{
char buf[2000]={'0'};
int size=sizeof(buf);
simul(buf,2000);
printf("\n--------------------------------------------------------------\n");
for(int i=0; i< (size-1); i++)
{
  printf("%c",buf[i]); 
}


}




