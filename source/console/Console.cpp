/** BBB Serial Slave Server
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root
* directory for copyright and GNU GPLv3 license information.            */

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
//#include<stdlib.h>
#include<cstdlib>
#include"AD820xx.h"
#include"../drivers/gpio/GPIO.h"
#include"../example/eeprom_i2c.h"

using namespace exploringBB;

extern void GPIOCtrl_SetOut(GPIO_VALUE value);
extern void GPIOCtrl_swingPort(void);

extern int sock;

//#define MODEMDEVICE "/dev/ttyO2" //Beaglebone Black serial port
#define MODEMDEVICE "/dev/ttyO0" //Beaglebone Black serial port
//#define MODEMDEVICE "/dev/ttyO4" //Beaglebone Black serial port
#define STATIC static

typedef struct
{
    char *CmdName;
    char *CmdHelp;
    unsigned char (*CmdHandler)(int client, unsigned char *Cmd, unsigned char *Parms);
}UART_CMDS;

int message(int client, const char *message);
STATIC unsigned char Console_cmdHelp(int client, unsigned char *Cmd, unsigned char *Parms);
STATIC unsigned char *RemoveSpaces (unsigned char *Command);
STATIC unsigned char Console_cmdAD82010(int client, unsigned char *Cmd, unsigned char *Parms);
STATIC unsigned char Console_setGPIO(int client, unsigned char *Cmd, unsigned char *Parms);
STATIC unsigned char Console_eeprom(int client, unsigned char *Cmd, unsigned char *Parms);
STATIC unsigned char Console_setMRX(int client, unsigned char *Cmd, unsigned char *Parms);
STATIC unsigned char Console_runBinary(int client, unsigned char *Cmd, unsigned char *Parms);

static const UART_CMDS UartCommands[] =
{
    {"?",       "Print this message",               Console_cmdHelp},
    {"help",    "Print this message",               Console_cmdHelp},
    {"gpio",    "gpio control for on/off",          Console_setGPIO},
    {"amp",     "control for DAMP ad82010",         Console_cmdAD82010},
    {"eeprom",  "save eeprom or load data",         Console_eeprom},
    {"mrx",     "mrx control setting",              Console_setMRX},
	{"bin",     "execute external binary",          Console_runBinary},
    {"",        "",                                 NULL}
};

STATIC unsigned char Console_runBinary(int client, unsigned char *Cmd, unsigned char *Parms)
{
	int status = 0;
    unsigned char RetVal = 0, uStep, value = 0;
    if(!strncmp((char *)Parms, "echo_server", uStep = strlen("echo_server")))
    {
        message(client, "*** execute external binary ***\n");
        printf("run echo_server\n\r");

        status = system("/home/rock/echo_server_r1 15000 &");
        printf("status:%d\n",status);

        // Create Child Processor
        /*if(fork() == 0){
			// Child process will return 0 from fork()
			printf("I'm the child process.\n");
			status = system("/home/rock/echo_server_r1 14999");
			printf("status:%d\n",status);
			exit(0);
		}else{
			// Parent process will return a non-zero value from fork()
			printf("I'm the parent.\n");
		}*/
    }
    else
    {
        message(client, "*** Unknown command! ***\n");
        printf("Unknown command!\n\r");
    }

    return (RetVal);
}

unsigned char binBuf[16] = {0,};
unsigned int AddrCnt = 0;
char printbuf[100] = {0,};
STATIC unsigned char Console_eeprom(int client, unsigned char *Cmd, unsigned char *Parms)
{
    unsigned char RetVal = 0, uStep, value = 0;
    unsigned char ReadBuf[16] = {0,};

    if(!strncmp((char *)Parms, "write", uStep = strlen("write")))
    {
        for(int cnt = 0; cnt < 16; cnt += 1)
        {
            binBuf[cnt] =rand()%256;
            printf("binBuf[%d]:%02X\n", cnt, binBuf[cnt]);
        }

        usleep(1000);
        EEPROM_WriteBufData(AddrCnt, binBuf, 16);
        message(client, "*** eeprom write ***\n");
        usleep(1000);
#if 1
        memset(printbuf, 0, sizeof(printbuf));
        sprintf(printbuf, "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",\
                          binBuf[0],binBuf[1],binBuf[2],binBuf[3],binBuf[4],binBuf[5],binBuf[6],binBuf[7],\
                          binBuf[8],binBuf[9],binBuf[10],binBuf[11],binBuf[12],binBuf[13],binBuf[14],binBuf[15]);
        //message(client, (char *)printbuf);
        printf("%s", printbuf);
#endif
    }
    else if(!strncmp((char *)Parms, "read", uStep = strlen("read")))
    {
        EEPROM_ReadBufData(AddrCnt, ReadBuf, 16);
        for(int cnt1 = 0; cnt1 < 16; cnt1 += 1)
        {
            printf("ReadBuf[%d]:%02X\n\r", cnt1, ReadBuf[cnt1]);
        }
        message(client, "*** eeprom read ***\n");
    }
    else if(!strncmp((char *)Parms, "rand", uStep = strlen("rand")))
    {
        for(int cnt = 0; cnt < 16; cnt += 1)
        {
            binBuf[cnt] =rand()%256;
            printf("binBuf[%d]:%02X\n", cnt, binBuf[cnt]);
        }
        message(client, "*** eeprom random ***\n");
    }
    else
    {
        message(client, "*** Unknown command! ***\n");
    }

    return (RetVal);
}


STATIC unsigned char Console_setGPIO(int client, unsigned char *Cmd, unsigned char *Parms)
{
    unsigned char RetVal = 0, uStep, value = 0;
    if(!strncmp((char *)Parms, "on", uStep = strlen("on")))
    {
        GPIOCtrl_SetOut(HIGH);
	message(client, "GPIO ON\n");
    }
    else if(!strncmp((char *)Parms, "off", uStep = strlen("off")))
    {
        GPIOCtrl_SetOut(LOW);
	message(client, "GPIO OFF\n");
    }
    else if(!strncmp((char *)Parms, "swing", uStep = strlen("swing")))
    {
        GPIOCtrl_swingPort();
	message(client, "GPIO SWING\n");
    }
    else
    {
        message(client, "*** Unknown command! ***\n");
    }

    return (RetVal);
}

STATIC unsigned char Console_cmdHelp(int client, unsigned char *Cmd, unsigned char *Parms)
{
    unsigned char i=0;
    char Spaces[16];

    Spaces[10]=0;

    //DBG("\n\rCommand   Description and parameters (All data in hex)\n\r\n\r");
    message(client,  (char *)"\n\rCommand   Description and parameters (All data in hex)\n\r\n\r");
    while (UartCommands[i].CmdHandler)
    {
        memset(Spaces, 0x20, 10);
        memcpy(Spaces, UartCommands[i].CmdName,
               strlen(UartCommands[i].CmdName));
        //DBG("%s", Spaces);
        message(client,  (char *)Spaces);

        //DBG("  %s\n\r", UartCommands[i].CmdHelp);
        //message("  %s\n\r", UartCommands[i].CmdHelp);
        message(client,  (char *)UartCommands[i].CmdHelp);
        message(client,  (char *)"\n");
        i++;
    }
    //DBG("\n\r");
    message(client,  (char *)"\n\r");
    return (0);
}

/*============================================================================
 *
 *
 *
 *===========================================================================*/
STATIC unsigned char Console_cmdAD82010(int client, unsigned char *Cmd, unsigned char *Parms)
{
    unsigned char RetVal = 0, uStep, value = 0;

    if(!strncmp((char *)Parms, "mvol", uStep = strlen("mvol")))
    {
      Parms += uStep + 1;

      if(!strncmp((char *)Parms, "mute", uStep = strlen("mute")))
      {
        Parms += uStep + 1;
        if(!strncmp((char *)Parms, "on", uStep = strlen("on")))
        {
          AD820xx_SetMasterMute(1);
        }
        else
        {
          AD820xx_SetMasterMute(0);
        }
      }
      else
      {
        if(strlen((char *)Parms) == 3)  // 3 digit (100)
        {
          value = (Parms[0]-0x30)*100;
          value += (Parms[1]-0x30)*10;
          value += (Parms[2]-0x30);
        }
        else if(strlen((char *)Parms) < 2)  // 1 digit (0~9)
        {
          value = Parms[0]-0x30;
        }
        else    // 2 digit
        {
          value = (Parms[0]-0x30)*10;
          value += (Parms[1]-0x30);
        }
        AD820xx_SetMasterVolume(value);
      }
    }
    else if(!strncmp((char *)Parms, "vol1", uStep = strlen("vol1")))
    {
      Parms += uStep + 1;

      if(!strncmp((char *)Parms, "mute", uStep = strlen("mute")))
      {
        Parms += uStep + 1;
        if(!strncmp((char *)Parms, "on", uStep = strlen("on")))
        {
          AD820xx_SetCh1Mute(1);
        }
        else
        {
          AD820xx_SetCh1Mute(0);
        }
      }
      else
      {
        if(strlen((char *)Parms) == 3)  // 3 digit (100)
        {
          value = (Parms[0]-0x30)*100;
          value += (Parms[1]-0x30)*10;
          value += (Parms[2]-0x30);
        }
        else if(strlen((char *)Parms) < 2)  // 1 digit (0~9)
        {
          value = Parms[0]-0x30;
        }
        else    // 2 digit
        {
          value = (Parms[0]-0x30)*10;
          value += (Parms[1]-0x30);
        }
        AD820xx_SetCh1Volume(value);
      }
    }
    else if(!strncmp((char *)Parms, "vol2", uStep = strlen("vol2")))
    {
      Parms += uStep + 1;

      if(!strncmp((char *)Parms, "mute", uStep = strlen("mute")))
      {
        Parms += uStep + 1;
        if(!strncmp((char *)Parms, "on", uStep = strlen("on")))
        {
          AD820xx_SetCh1Mute(1);
        }
        else
        {
          AD820xx_SetCh1Mute(0);
        }
      }
      else
      {
        if(strlen((char *)Parms) == 3)  // 3 digit (100)
        {
          value = (Parms[0]-0x30)*100;
          value += (Parms[1]-0x30)*10;
          value += (Parms[2]-0x30);
        }
        else if(strlen((char *)Parms) < 2)  // 1 digit (0~9)
        {
          value = Parms[0]-0x30;
        }
        else    // 2 digit
        {
          value = (Parms[0]-0x30)*10;
          value += (Parms[1]-0x30);
        }
        AD820xx_SetCh2Volume(value);
      }
    }
    else if(!strncmp((char *)Parms, "mix", uStep = strlen("mix")))
    {
      Parms += uStep + 1;

      if(!strncmp((char *)Parms, "ch1", uStep = strlen("ch1")))
      {
        Parms += uStep + 1;
        if(!strncmp((char *)Parms, "enable", uStep = strlen("enable")))
        {
          AD820xx_CH1MixingEnable(1);
        }
        else
        {
          AD820xx_CH1MixingEnable(0);
        }
      }
      else if(!strncmp((char *)Parms, "ch2", uStep = strlen("ch2")))
      {
        Parms += uStep + 1;
        if(!strncmp((char *)Parms, "enable", uStep = strlen("enable")))
        {
          AD820xx_CH2MixingEnable(1);
        }
        else
        {
          AD820xx_CH2MixingEnable(0);
        }
      }
      else
      {
        RetVal = 1;
      }
    }
    else if(!strncmp((char *)Parms, "mono", uStep = strlen("mono")))
    {
      Parms += uStep + 1;
      if(!strncmp((char *)Parms, "enable", uStep = strlen("enable")))
      {
        AD820xx_SetMonoEnable(1);
      }
      else
      {
        AD820xx_SetMonoEnable(0);
      }
    }
    else
    {
        RetVal = 1;
    }
    return (RetVal);
}

STATIC unsigned char Console_setMRX(int client, unsigned char *Cmd, unsigned char *Parms)
{
    unsigned char RetVal = 0, uStep, value = 0;
    if(!strncmp((char *)Parms, "volup", uStep = strlen("volup")))
    {
        message(client, "*** MRX VOLUME UP ***\n");
        printf("MRX VOLUME UP\n\r");
    }
    else if(!strncmp((char *)Parms, "voldw", uStep = strlen("voldw")))
    {
        message(client, "*** MRX VOLUME DOWN ***\n");
        printf("MRX VOLUME DOWN\n\r");
    }
    else if(!strncmp((char *)Parms, "mute", uStep = strlen("mute")))
    {
        message(client, "*** MUTE TOGGLE ***\n");
        printf("MUTE TOGGLE\n\r");
    }
    else if(!strncmp((char *)Parms, "cmd", uStep = strlen("cmd")))
    {
        Parms += uStep+1;
        message(client, "CMD\n");
        write(sock, (char*)Parms, strlen((char*)Parms));
        printf("CMD:%s\n\r", (char*)Parms);
    }
    else
    {
        message(client, "*** Unknown command! ***\n");
        printf("Unknown command!\n\r");
    }

    return (RetVal);
}
// Sends a message to the client and displays the message on the console
int message(int client, const char *message)
{
   int size = strlen(message);
   printf(message);
   if (write(client, message, size)<0){
      perror("Failed to write to the client\n");
      return -1;
   }
   return 0;
}

/*============================================================================
 *
 *
 *
 *===========================================================================*/
static unsigned char *RemoveSpaces (unsigned char *Command)
{
    while (*Command == ' ')
    {
        Command++;
    }
    return (Command);
}

static void RemoveNonASCII(char *buffer, unsigned int length)
{
	while(length)
	{
		length -= 1;
		if(buffer[length] < ' ')
		{
			buffer[length] = 0;
		}
	}
}


/*============================================================================
 *
 *
 *
 *===========================================================================*/
void CommandHandler(int client, unsigned char *Command)
{
    int return_val = -1;
    unsigned char i=0, Len, *Cmd;

    RemoveNonASCII((char*)Command, strlen((char*)Command));

    Command = RemoveSpaces (Command);
    while (UartCommands[i].CmdHandler)
    {
        Len = strlen (UartCommands[i].CmdName);
        if (!strncmp((char *)Command, UartCommands[i].CmdName, Len))
        {
            Cmd = Command;
            if ((Cmd[Len] == 0) || (Cmd[Len] == ' '))
            {
                Command = RemoveSpaces (Cmd+Len);
                if (UartCommands[i].CmdHandler(client, Cmd, Command))
                {
                    //DBG("Command execution failed\n\r");
                    message(client, (char *)"Command execution failed\n\r");
                }
                break;
            }
        }
        i++;
    }
    if ((!UartCommands[i].CmdHandler) && (Command[0] >= ' '))
    {
        //DBG("Unknown command\n\r");
        message(client, (char *)"Unknown command\n\r");
    }
}


#if 1
// Checks to see if the command is one that is understood by the server
int processBBBCommand(int client, char *command){
   int return_val = -1;
   //if (strcmp(command, "LED on")==0){
   if(!strncmp((char *)command, "LED on", strlen("LED on"))) {
      return_val = message(client, (char *)"*** Turning the LED on  ***\n");
      //printf("*** Turning the LED on  ***\n");
   }
   else if(!strncmp((char *)command, "LED off", strlen("LED off"))) {
      return_val = message(client, "*** Turning the LED off ***\n");
//      printf("*** Turning the LED off  ***\n");
   }
   else if(strncmp((char *)command, "quit", strlen("quit"))) {
      return_val = message(client, "*** Killing the BBB Serial Server ***\n");
//      printf("*** Killing the BBB Serial Server ***\n");
   }
   else {
      return_val = message(client, "*** Unknown command! ***\n");
//      printf("*** Unknown command!***\n");
   }
   return return_val;
}
#endif

int client = 0;
void Console_msgParser(char *command)
{
    CommandHandler(client, (unsigned char *)command);
}

// The main application. Must be run as root and must pass the terminal name.
void* Console_TaskMain(void *pArg)
{
   int /*client,*/ count=0;
   unsigned char c;
   unsigned char *command = (unsigned char *)malloc(255);
//   char *command = (char *)malloc(255);
#if 0
   // Is the user root?
   if(getuid()!=0){
      perror("You must run this program as root on the BBB. Exiting!\n");
      return NULL;
   }
#endif
   // Set up the connection
   if ((client = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return NULL;
   }
   struct termios options;
   tcgetattr(client, &options);
   options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   tcflush(client, TCIFLUSH);
   fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);  // make the reads non-blocking
   tcsetattr(client, TCSANOW, &options);
   if (message(client, "BBB Serial Server has started running.\n")<0){
      return NULL;
   }

   // Loop forever until the quit command is sent from the client or
   //  Ctrl-C is pressed on the server console
   do {
      if(read(client,&c,1)>0){
          write(STDOUT_FILENO,&c,1);
          command[count++]=c;
          if(c=='\n'){
             command[count-1]='\0';   //replace /n with /0
             //processBBBCommand(client, command);
             CommandHandler(client, (unsigned char *)command);
             count=0;                 //reset the command string for the next command
          }
      }
      if(read(STDIN_FILENO,&c,1)>0){  //can send text from stdin to client machine
          write(client,&c,1);
      }
   }
   while(strcmp((char *)command,"quit")!=0);
   free(command);
   command = NULL;

#if 0 /* yhcha, Jun 15 2020 : Block */
   //while(1)
   {
       // Loop forever until the quit command is sent from the client or
       //  Ctrl-C is pressed on the server console
       do {
          if(read(client,&c,1)>0){
              write(client,&c,1);
              command[count++]=c;
              if(c=='\n' || c=='\r'){
                 command[count-1]='\0';   //replace /n with /0
//                 processBBBCommand(client, (char *)command);
                 CommandHandler(client, command);
                 count=0;                 //reset the command string for the next command
                 message(client, "*** count reset***\n");
              }
          }
          if(read(STDIN_FILENO,&c,1)>0){  //can send text from stdin to client machine
          //if(read(client,&c,1)>0){  //can send text from stdin to client machine
              write(client,&c,1);
          }
       }
       while(strcmp((char *)command,"quit")!=0);
       close(client);

	free(command);
	command = NULL;
   }
#endif

   return 0;
}
