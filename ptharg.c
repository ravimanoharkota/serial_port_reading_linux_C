
//https://pages.cs.wisc.edu/~remzi/Classes/537/Fall2008/Notes/threads-semaphores.txt
//pthread condition vars: https://gist.github.com/rtv/4989304
//arg thrd index issue and thread info : https://man7.org/linux/man-pages/man3/pthread_create.3.html
//codevault

#include "ptharg.h"

int done = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

       struct thread_info {    /* Used as argument to thread_start() */
           pthread_t thread_id;        /* ID returned by pthread_create() */
           int       thread_num;       /* Application-defined thread # */
           char     *argv_string;      /* From command-line argument */
       };

int buffer[10];
int count = 0;

#define READER_THREAD_NUM 8

sem_t semEmpty[READER_THREAD_NUM];
//sem_t semFull;

sem_t semFull[READER_THREAD_NUM];

sem_t room;

/*===================
READING
------*/
uint8_t app_rx_buf[APP_RX_BUFF_SIZE]; //buffer for RX application thread 

/*
int buffer_get(uint8_t* app_rx_buf, size_t *app_rx_bufoffset)
{

    int numbytesdata, sourcebufstart;
    static size_t app_rx_bufoffset=0; 
    // Reader acquire the cbuf-mutex before accessing cbuf 
    pthread_mutex_lock(&cbuf_mutex);
    numbytesdata = s->reader_end - s->reader_start + 1;
    sourcebufstart = s->reader_start;
   
    int next = *app_rx_bufoffset;
    for (int i=0; i< numbytesdata; i++)
    {
               (next < APP_RX_BUFF_SIZE)? (next): (next=0);   
                app_rx_buf[next++] = s->rxbuff[sourcebufstart];
                //me->tail = advance_headtail_value(me->tail, me->max);
                //me->full = false;
                //r = 0;
               

    }

    pthread_mutex_unlock(&cbuf_mutex);

    app_rx_buf[(*app_rx_bufoffset)++]= '\0'; //Null Terminate
    return numbytesdata;
}

*/


void *hello(void *input) 
{


  struct thread_info *tinfo = input;
  int consumer_id =  tinfo->thread_num;
  int res=-1;

  while (1) {
        int y;
        static size_t app_rx_bufoffset = 0;

//printf("reader %d is waiting\n", consumer_id);
   //   sem_wait(&room); 
        // Remove from the buffer
        //sem_wait(&semFull);
        sem_wait(&semFull[consumer_id]);
        //printf("%d\n trying to acquire mutex", consumer_id);
        pthread_mutex_lock(&mutex);
        //y = buffer[count - 1];
        //count--;

#if 0
   if ( res = buffer_get(app_rx_buf, &app_rx_bufoffset)) {
       printf("reader %d read %d bytes", consumer_id, res);
    }
    else {
       printf("ERROR: reader %d read %d bytes", consumer_id, res);
    }
#endif

#if 0 //DEQ simulation
    _uart_pkt_buf uart_pkt_buf_out={0};
    for(int i=0; i<20; i++) {
      if ( (res=dequeue(&uart_pkt_buf_out)) < 0)
      {   
        printf("dequeu error in main()");
      }   
      memset(&uart_pkt_buf_out, '\0', sizeof(_uart_pkt_buf));
    }   
#endif

 //DEQUEUE
       memset(app_rx_buf, '\0', APP_RX_BUFF_SIZE);
       res = ringbuf_deque_multiple(app_rx_buf, APP_RX_BUFF_SIZE);
       if(res <0)
       {
         printf("ringbuf_dequeue_multiple() failed");
       }
        printf("reader:%d acquired mutex, read:\n", consumer_id);
        
        //printf("reader:%d acquired mutex, read pktid:%d\n", consumer_id, uart_pkt_buf_out.pktid);
        pthread_mutex_unlock(&mutex);

        ///database_record_data(app_rx_buf, APP_RX_BUFF_SIZE);

        //if not the last reader, then signal the next reader in chain
        if(consumer_id < (READER_THREAD_NUM -1))
        {
         //sem_post(&semEmpty);
         sem_post(&semFull[consumer_id+1]);
        }
        else
        {//last reader, signal the writer back again
        // sem_post(&semEmpty[0]);

        sem_wait(&semFull[0]); //block the reader 0 until writer is done next //Can we do this ?
        // count--; //only last reader manages count, back for writer 
        }

        // Consume
        //printf("Got %d\n", y);
     // sem_post(&room); 
        sleep(1);
    }

}//reader()

void *writer(void *writeridx)
{
static int data =0;
 static int pkt_count=0;
int res=-1;

    while (1) {
        // Produce
        //int x = rand() % 100;
        //int x = ++data; //the data to enqueue
        sleep(10); //sleep longer to ensure all readers run

        // Add to the buffer
       // sem_wait(&semEmpty);
   
    //  sem_wait(&room);// 
    //    sem_wait(&semEmpty[0]);  //block the 1st reader
        pthread_mutex_lock(&mutex);

    //ENQUE 
       build_packet_to_simulate_enque(app_rx_buf, APP_RX_BUFF_SIZE);
       app_rx_buf[APP_RX_BUFF_SIZE] ='\0';
       res = ringbuf_enque_multiple(app_rx_buf, APP_RX_BUFF_SIZE);
       if(res <0)
       {
         printf("ringbuf_enque_multiple() failed");
       }

        printf("writer:%d ringbuf_enque_multiple sucess! \n",*((int *)writeridx)); 
#if 0 //simulation - TODO replace by real serial.read()
    _uart_pkt_buf uart_pkt={0};
   // for(int i=0; i<20; i++) {
      memset(&uart_pkt, '\0', sizeof(_uart_pkt_buf));
      uart_pkt.len = 50;
      uart_pkt.pktid = ++pkt_count;
      sprintf(uart_pkt.uart_pkt_data, "packet:%d greets you - Hello user", uart_pkt.pktid);
      uart_pkt.uart_pkt_data[50] = '\0';


  res = enqueue_application_uart_packet(&uart_pkt);
  if(res < 0)
  {
    printf("ENQUEE ERROR");
    return;
  }
  memset(&uart_pkt, '\0', sizeof(_uart_pkt_buf));

    //}
        printf("writer %d enqueued pktid:%d\n", *((int *)writeridx), uart_pkt.pktid);
#endif //ringbuf_enque


       // buffer[count] = x;
       // count++;
        pthread_mutex_unlock(&mutex);
        sem_post(&semFull[0]); //signal the 1st readera
    }

}

#if 0
int main(void) {
    pthread_t tid[READER_THREAD_NUM], writer_thread,single_reader;
    int id = 6655;
    int i;

    //Init the ring buffer
    ringbuf_init();

    //display();

   sem_init(&semEmpty[i], 0, 0);
   sem_init(&semFull[i], 0, 0);

   struct thread_info *tinfo = calloc(3, sizeof(*tinfo));

    //Create Reader Threads
    //  pthread_create(&single_reader, NULL, hello, tinfo);
    for(int i=0; i<READER_THREAD_NUM; i++)
    {
      tinfo[i].thread_num = i;
      if ( 0 != (pthread_create(&tid[i], NULL, hello,  &tinfo[i])))
      {
        printf("Failed to create thread %d", i);
      }
      sleep(2);
    }
    
      //Create Writer Thread
      pthread_create(&writer_thread, NULL, writer, &id);
      printf("created threads");
      
      sleep(40); //wait for writer to complete 

     //Join all threads, close program
     for(int i=0; i<READER_THREAD_NUM; i++)
       pthread_join(tid[i], NULL);
      //   pthread_join(single_reader, NULL);
      pthread_join(writer_thread, NULL);

     
     display();


for(i=0; i<READER_THREAD_NUM; i++)
{
      sem_destroy(&semEmpty[i]);
      sem_destroy(&semFull[i]);
}
      pthread_mutex_destroy(&mutex);

    return 0;
}
#endif 

void main()
{
   int res;

  printf("3");
    ringbuf_init();


    print_address();
      // memset(app_rx_buf, '\0', APP_RX_BUFF_SIZE);

     build_packet_to_simulate_enque(app_rx_buf, APP_RX_BUFF_SIZE);

       app_rx_buf[APP_RX_BUFF_SIZE - 1] ='\0';
       //res = ringbuf_enque_multiple(app_rx_buf, APP_RX_BUFF_SIZE);

/*      res = enqueue(app_rx_buf);
       if(res <0)
       {
         printf("ringbuf_enque_multiple() failed");
       }
*/
//        print_address();
 
}



/* OUTPUT 
ravi@hpPavilion:~/rtes$ ./a.out 
writer:9 is writing 1
reader:1990 acquired mutex, read: 1Got 1
writer:9 is writing 2
reader:1990 acquired mutex, read: 2Got 2
writer:9 is writing 3
reader:1990 acquired mutex, read: 3Got 3
writer:9 is writing 4
reader:1990 acquired mutex, read: 4Got 4
writer:9 is writing 5
reader:1990 acquired mutex, read: 5Got 5
writer:9 is writing 6
reader:1990 acquired mutex, read: 6Got 6
writer:9 is writing 7
reader:1990 acquired mutex, read: 7Got 7
writer:9 is writing 8
reader:1990 acquired mutex, read: 8Got 8
writer:9 is writing 9
reader:1990 acquired mutex, read: 9Got 9
writer:9 is writing 10
reader:1990 acquired mutex, read: 10Got 10
writer:9 is writing 11
reader:1990 acquired mutex, read: 11Got 11
writer:9 is writing 12
reader:1990 acquired mutex, read: 12Got 12
writer:9 is writing 13
reader:1990 acquired mutex, read: 13Got 13
writer:9 is writing 14
reader:1990 acquired mutex, read: 14Got 14
writer:9 is writing 15
reader:1990 acquired mutex, read: 15Got 15
writer:9 is writing 16
reader:1990 acquired mutex, read: 16Got 16
writer:9 is writing 17
reader:1990 acquired mutex, read: 17Got 17
writer:9 is writing 18
reader:1990 acquired mutex, read: 18Got 18
writer:9 is writing 19
reader:1990 acquired mutex, read: 19Got 19
writer:9 is writing 20
reader:1990 acquired mutex, read: 20Got 20
writer:9 is writing 21
reader:1990 acquired mutex, read: 21Got 21
writer:9 is writing 22
reader:1990 acquired mutex, read: 22Got 22
writer:9 is writing 23
reader:1990 acquired mutex, read: 23Got 23
writer:9 is writing 24
reader:1990 acquired mutex, read: 24Got 24
writer:9 is writing 25
reader:1990 acquired mutex, read: 25Got 25
writer:9 is writing 26
reader:1990 acquired mutex, read: 26Got 26
writer:9 is writing 27
reader:1990 acquired mutex, read: 27Got 27
writer:9 is writing 28
reader:1990 acquired mutex, read: 28Got 28
writer:9 is writing 29
reader:1990 acquired mutex, read: 29Got 29
^Z
[5]+  Stopped                 ./a.out
ravi@hpPavilion:~/rtes$ 
*/
