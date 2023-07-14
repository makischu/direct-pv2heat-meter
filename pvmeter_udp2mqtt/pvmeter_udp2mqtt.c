//02.08.22 makischu as-is
//03.01.23 V02 for Hardware Version 02
//part of direct-pv2heat-meter. bridge between (low-level) udp/binary and (higher-level) mqtt/json. 
//to be (compiled and) run on a (linux) server, e.g. along with an mqtt broker.

// uses: paho mqtt https://github.com/eclipse/paho.mqtt.c
// parts copied from https://github.com/eclipse/paho.mqtt.c/blob/master/src/samples/MQTTClient_subscribe.c

// build:: gcc ...c -lpaho-mqtt3c  
//if working, including a local mqtt broker, status will be received every second, e.g. by:
// mosquitto_sub -h localhost -p 1883 -t dev/pvmeter/telemetry 
// {leCmdNr:104,leCmd:"ON",Sta:00110000_00000010,TiC:27.37,TeC:11.87,UoV: 20.38,IoA: 0.579,US1V: 11.32,U24V:11.86,PaltW:   2.9,a-6:-0.250000,b:0.140,rssiW:-79,rssiB:-86,nConW:  1,nConB:  1,tBrms:10315126}
// {leCmdNr:104,leCmd:"ON",Sta:00110000_00000010,TiC:27.37,TeC:11.87,UoV: 20.19,IoA: 0.586,US1V: 11.32,U24V:12.53,PaltW:   2.9,a-6:-0.250000,b:0.140,rssiW:-78,rssiB:-87,nConW:  1,nConB:  1,tBrms:10316115}
//
//activating the pwoer-output is possible e.g. by
// mosquitto_pub -h locahost -p 1883 -t dev/pvmeter/command -m "POWEROUT_ON"

#include <stdio.h> //printf
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h> //exit
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>

#include <errno.h>
#include <string.h>//memset
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <arpa/inet.h>

#include <time.h>

#include<signal.h>
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

volatile bool run = true;

void sig_handler(int signo)
{
  if (signo == SIGINT  || signo == SIGQUIT) 
    printf("received SIGINT\n");
  run=false;
}

#include "MQTTClient.h" //see above for installtion (clone local, make, sudo make install)

#define ADDRESS     "tcp://192.168.x.x:1883"  //"tcp://localhost:1883"
#define CLIENTID    "pvmeter_udp2mqtt"
#define TOPIC_STATUS "dev/pvmeter/telemetry"
#define TOPIC_CMD    "dev/pvmeter/command"
#define QOS         0
#define TIMEOUT     10000L
#define USERTOKEN   "pvmudp2mqtt"


#define BUFLEN 1024		    //Max length of buffer
#define PORT_LOCAL   1234	//The port on which to listen for incoming udp data
#define PORT_REMOTE  1234	//The port on which to send outgoing udp data
#define IP_REMOTE    "192.168.x.x" // The ip to talk to


uint8_t lastCmdNr=0;
uint8_t lastCmdCode=0;
volatile uint8_t cmdToSend=0;

// outputs - same as defined at sender.
struct __attribute__ ((packed)) Outputs {
  uint16_t  Status;   //0:relaisSet,1:relaisReset;2:fetS1On,3:fetS2On,Info,Error,DigIn1,DigIn2.
                      //8:relaisSetForced,9:RelaisResetForced,...
  uint16_t  TempIntcC;
  uint16_t  TempExtcC;
  uint16_t  UoutcV;
  int16_t  IoutmA;
  uint16_t  U24mV;
  uint16_t  US1cV;
  uint16_t  PwrAlttW;  //not used anymore. kept to avoid interface change.
  uint16_t  a_inv_em9; //not used anymore. inverted, times e+9.
  uint8_t   b_di2_em3; //not used anymore. divided/2, times e+3.
  uint8_t   LastReceivedCmdNr;
}; //max 20 bytes.


int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    char command[32]; memset(command, 0, sizeof(command));
    uint8_t cmdCode=0;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);

    strncpy(command, (char*)message->payload, sizeof(command)-2);
    if (strcmp(command,"RELAIS_SET")==0) {
        cmdCode = 0xC5;
    }
    if (strcmp(command,"RELAIS_RESET")==0) {
        cmdCode = 0xC8;
    }
    if (strcmp(command,"RELAIS_AUTO")==0) {
        cmdCode = 0xCA;
    }
    if (strcmp(command,"POWEROUT_ON")==0) {
        cmdCode = 0xC1;
    }
    if (strcmp(command,"POWEROUT_OFF")==0) {
        cmdCode = 0xC0;
    }
    if (strcmp(command,"REBOOT")==0) {
        cmdCode = 0xCB;
    }
    if (strcmp(command,"OTA")==0) {
        cmdCode = 0xCF;
    }
    if (cmdCode != 0) {
        cmdToSend = cmdCode;
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}


void formatStatus(char* bufOut, char* bufIn) {
    uint8_t* pBufIn=(uint8_t*)bufIn;
    struct Outputs* pS;
    int8_t rssiWifi;
    int8_t rssiBLE;
    uint8_t nrConnectsWifi;
    uint8_t nrConnectsBLE;
    uint32_t t_send;
    pS = (struct Outputs*)(pBufIn+4);
    pBufIn+=4+sizeof(struct Outputs);
    rssiWifi =  *((int8_t*)pBufIn++);
    rssiBLE  = *((int8_t*)pBufIn++);
    nrConnectsWifi = *pBufIn++;
    nrConnectsBLE = *pBufIn++;
    t_send = *((uint32_t*)pBufIn); pBufIn+=4;
    char sStatus[18]={0}; char* pSt=sStatus;
    for(int i=0;i<16;i++) {
        if (i==8) *(pSt++) = '_';
        *(pSt++) = pS->Status&(1<<(15-i))?'1':'0';
    }

    const char* lastCmdShort = "__";
    if(pS->LastReceivedCmdNr == lastCmdNr) {
        switch(lastCmdCode) {
        case 0xC5: lastCmdShort = "SE"; break;
        case 0xC8: lastCmdShort = "RE"; break;
        case 0xCA: lastCmdShort = "AU"; break;
        case 0xC1: lastCmdShort = "ON"; break;
        case 0xC0: lastCmdShort = "OF"; break;
        case 0xCB: lastCmdShort = "RB"; break;
        case 0xCF: lastCmdShort = "UP"; break;
        default:  break;
        }
    } 

    sprintf(bufOut, "{leCmdNr:%2d,leCmd:\"%s\",Sta:%s,TiC:%4.2f,TeC:%4.2f,UoV:%6.2f,IoA:%6.3f,US1V:%6.2f,U24V:%5.2f,PaltW:%6.1f,a-6:%f,b:%5.3f,rssiW:%3d,rssiB:%3d,nConW:%3d,nConB:%3d,tBrms:%8d}",
        pS->LastReceivedCmdNr, lastCmdShort, sStatus, 
        pS->TempIntcC==0x8000?-9.0:pS->TempIntcC*0.01f, pS->TempExtcC==0x8000?-9.0:pS->TempExtcC*0.01f, 
        pS->UoutcV * 0.01f, pS->IoutmA * 0.001f, pS->US1cV * 0.01f, pS->U24mV * 0.001f, pS->PwrAlttW * 0.1f,
        pS->a_inv_em9 * -1e-9 *1e6, pS->b_di2_em3 * 2e-3,
        rssiWifi, rssiBLE, nrConnectsWifi, nrConnectsBLE, t_send);
} 



int main(int argc, char *argv[]) {
    struct sockaddr_in si_me, si_sender,si_dest;
	int s, i, slen = sizeof(si_sender) , recv_len, sent_len;
	char bufIn[BUFLEN];
    char bufOut[BUFLEN];
    struct timeval read_timeout;
    time_t tlastrx, tnow;
    tlastrx = tnow = time(0);
     MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
	
	//create a UDP socket
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		printf("socket fail");
        return 1;
	}
	
	// zero out the structure
	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT_LOCAL);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    memset((char *)&si_dest,0,sizeof(si_dest));
	si_dest.sin_family = AF_INET;
	si_dest.sin_port = htons(PORT_REMOTE);
	si_dest.sin_addr.s_addr = inet_addr(IP_REMOTE);
    
	
	//bind socket to port
	if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
	{
		printf("bind fail");
        return 1;
	}

    //send initial message - if up it will cause the bridge to learn our own ip to send data to.
    sprintf(bufOut, "SEND Hello");
    if ((sent_len=sendto(s,bufOut,strlen(bufOut)+1,0,(const struct sockaddr *) &si_dest,slen)) ==-1)
    {
		printf("initial send fail");
        return 1;
    }  

    //https://stackoverflow.com/questions/15941005/making-recvfrom-function-non-blocking
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 100000;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
         printf("Failed to create client, return code %d\n", rc);
         return 1;
    }

    if ((rc = MQTTClient_setCallbacks(client, NULL, NULL, msgarrvd, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to set callbacks, return code %d\n", rc);
        return EXIT_FAILURE;
    }


    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = USERTOKEN;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }


    printf("Subscribing to topic %s\nfor client %s using QoS%d\n", TOPIC_CMD, CLIENTID, QOS);
    if ((rc = MQTTClient_subscribe(client, TOPIC_CMD, QOS)) != MQTTCLIENT_SUCCESS)
    {
    	printf("Failed to subscribe, return code %d\n", rc);
    	exit(EXIT_FAILURE);
    }


  if (signal(SIGINT, sig_handler) == SIG_ERR)
    printf("\ncan't catch SIGINT\n");
  if (signal(SIGQUIT, sig_handler) == SIG_ERR)
    printf("\ncan't catch SIGQUIT\n");

    printf("sizeof(struct Outputs)=%d\n", sizeof(struct Outputs));

	//keep listening for data
	while(run)
	{
        tnow = time(0);
		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, bufIn, BUFLEN, 0, (struct sockaddr *) &si_sender, &slen)) != -1)
		{
            tlastrx = tnow;

            if(recv_len >= 4+sizeof(struct Outputs)+8 && memcmp(bufIn,"RCVD",4)==0) {

                formatStatus(bufOut, bufIn);

                pubmsg.payload = bufOut;
                pubmsg.payloadlen = (int)strlen(bufOut)+1;
                pubmsg.qos = QOS;
                pubmsg.retained = 0;
                if ((rc = MQTTClient_publishMessage(client, TOPIC_STATUS, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
                {
                    printf("Failed to publish message, return code %d\n", rc);
                    //exit(EXIT_FAILURE);
                    // Failed to publish message, return code -1   already seen.
                    break;
                } 
            }
            else {
                printf("Received a message I could not parse of length %d: %s\n",recv_len,bufIn);
            }
		}
        if(difftime(tnow,tlastrx)>10.0) {
            //after 10s of no received frame, send one to animate the bridge to learn our address
            tlastrx = tnow;
            sprintf(bufOut, "SEND Hello again");
            if ((sent_len=sendto(s,bufOut,strlen(bufOut)+1,0,(const struct sockaddr *) &si_dest,slen)) ==-1)
            {
                printf("repeated send fail");
                break;
            }  
        }

        if (cmdToSend != 0) {
            uint8_t cmdNr;
            cmdNr = lastCmdNr+1;
            if (cmdNr==255) cmdNr=1;
            bufOut[0] = 'S';
            bufOut[1] = 'E';
            bufOut[2] = 'N';
            bufOut[3] = 'D';
            bufOut[4] = cmdNr;
            bufOut[5] = cmdToSend;
            bufOut[6] = cmdNr ^ cmdToSend;
            bufOut[7] = 0;

            if ((sent_len=sendto(s,bufOut,strlen(bufOut)+1,0,(const struct sockaddr *) &si_dest,slen)) ==-1)
            {
                printf("udp send fail");
               break;
            }  
            lastCmdCode = cmdToSend;
            lastCmdNr = cmdNr;
            cmdToSend = 0;
        }


    }

    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    	printf("Failed to disconnect, return code %d\n", rc);
    MQTTClient_destroy(&client);

    close(s); 
    return 0;
} 
