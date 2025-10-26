# serial_port_reading_linux_C

This is a small academic demonstrative project to constantly read data from serial port in Linux as long time as it is available, and post the data through ringbuffer, and then spawn multiple reader threads concurrently running that read data into a linear application buffer ( appbuf[] ), finally another single writer thread to consume from this application buffer and write data to a database file.

Multiple readers read ONE AFTER THE OTHER signalling the next in sequence AFTER the current is done reading using semaphores signal and wait API, and ensuring that only the first reader blocks out the db writer entirely, and last reader enables it to proceed with db recording. This demonstrates implementation of reader-writer problem and semaphore-based signalling among multiple pthreads. The readers read continued data sequentially and not duplicate copies. The readers are intentionally NOT launched concurrently, to demonstrate semaphore signalling and waiting for academic purpose.

Environment: 
 1. Linux system programming and user-space
 2. H/w serial port accessing through system calls
 3. Arduino as end hardware device

Test Setup:

1. H/w device:  Ardunio was used, that was programmed with a base sketch that sends timed data to serial port. The base     sketch is in baremetal Embedded Cpp code.
2. Linux ubuntu 22.04 LTS linux laptop, as both dev machine and recieving host.

Output file:

db.txt on ubuntu machine in a specific path.

Test Result:

Tested successfully on my Ubuntu 22.04 LTS linux laptop and result PASS.

Future Work:
1. Code cleanup and refactoring
2. Upgrade writer thread to Send data over network and have DB recording at remote server
   ( Wireless storage implementation )
