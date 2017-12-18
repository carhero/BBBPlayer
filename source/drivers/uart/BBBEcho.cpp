#if 0

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <unistd.h>

void* uart_TaskMain(void* pArg);

/* baudrate settings are defined in <asm/termbits.h>, which is
   included by <termios.h> */
#define BAUDRATE B115200   // Change as needed, keep B
#define BAUDRATE B115200

/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyO0" //Beaglebone Black serial port
//#define MODEMDEVICE "/dev/ttyO3" //Beaglebone Black serial port

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

char testText[] = "hello world nice to meet you\n\r";

void* uart_TaskMain(void* pArg)
{
    int file, count;
#if 0
    if(argc!=2){
       printf("Invalid number of arguments, exiting!\n");
       return -2;
    }
#endif
    if ((file = open("/dev/ttyO2", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return NULL;
    }
    struct termios options;
    tcgetattr(file, &options);
    //options.c_cflag = B57600 | CS8 | CREAD | CLOCAL;
    options.c_cflag = BAUDRATE | CS8 | CREAD | CLOCAL;
    options.c_iflag = IGNPAR | ICRNL;
    tcflush(file, TCIFLUSH);
    tcsetattr(file, TCSANOW, &options);

    // send the string plus the null character
    if ((count = write(file, testText, strlen(testText)+1))<0){
      perror("Failed to write to the output\n");
      return NULL;
    }
    //usleep(100000);
    char receive[100];

    while(1)
    {
        if(count = read(file, (void*)receive, sizeof(receive)) > 0)
        {
            printf("The following was read in [%d]: %s\n",count,receive);
            write(file, receive, strlen(receive)+1);
            write(file, "\r", strlen("\r")+1);
        }
    }
//    if ((count = read(file, (void*)receive, 100))<0){
//      perror("Failed to read from the input\n");
//      return NULL;
//    }
//    if (count==0) printf("There was no data available to read!\n");
//    else {
//      receive[count]=0;  //There is no null character sent by the Arduino
//      printf("The following was read in [%d]: %s\n",count,receive);
//    }
    close(file);
    return 0;
}


#if 0
void* uart_TaskMain(void* pArg)
{
    int fd, c, res;
    struct termios oldtio, newtio;
    char buf[255];
    // Load the pin configuration
    //int ret = system("echo uart2 > /sys/devices/bone_capemgr.9/slots");
    /* Open modem device for reading and writing and not as controlling tty
       because we don't want to get killed if linenoise sends CTRL-C. */
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
    if (fd < 0) { perror(MODEMDEVICE); exit(-1); }

    bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

    /* BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
       CRTSCTS : output hardware flow control (only used if the cable has
                 all necessary lines. See sect. 7 of Serial-HOWTO)
       CS8     : 8n1 (8bit,no parity,1 stopbit)
       CLOCAL  : local connection, no modem contol
       CREAD   : enable receiving characters */
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

    /* IGNPAR  : ignore bytes with parity errors
       otherwise make device raw (no other input processing) */
    newtio.c_iflag = IGNPAR;

    /*  Raw output  */
    newtio.c_oflag = 0;

    /* ICANON  : enable canonical input
       disable all echo functionality, and don't send signals to calling program */
    newtio.c_lflag = ICANON;
    /* now clean the modem line and activate the settings for the port */
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);
    // NMEA command to ouput all sentences
    // Note that this code & format values in manual are hexadecimal
    write(fd, "$PTNLSNM,273F,01*27\r\n", 21);
    write(fd, "ANAM ELECTRONICS\r\n", 18);
    write(fd, "YHCHA\r\n", 7);
    write(fd, "HELLO WORLD\r\n", 13);
    write(fd, "$PTNLSNM,273F,01*27\r\n", 21);
    /* terminal settings done, now handle input*/
    while (TRUE) {     /* loop continuously */
        /*  read blocks program execution until a line terminating character is
            input, even if more than 255 chars are input. If the number
            of characters read is smaller than the number of chars available,
            subsequent reads will return the remaining chars. res will be set
            to the actual number of characters actually read */
        res = read(fd, buf, 255);
        buf[res] = 0;             /* set end of string, so we can printf */
        printf("%s", buf, res);
    }
    tcsetattr(fd, TCSANOW, &oldtio);
}
#endif

#endif
