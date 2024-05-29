/* @brief This is a simple application for testing SPI communication on a RedPitaya
*
* (c) Red Pitaya  http://www.redpitaya.com
*
* This part of code is written in C programming language.
* Please visit http://en.wikipedia.org/wiki/C_(programming_language)
* for more details on the language used herein.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rp_hw.h"


int ahe_SPI(){
	
	
}

int init_SPI(){
	//Author: Greg Lusk
	//notes: code tested for SPI clock speed 
	
    int res = rp_SPI_InitDevice("/dev/spidev1.0"); // Init spi api.
    printf("Init result: %d\n",res);

    res = rp_SPI_SetDefaultSettings(); // Set default settings.
    printf("Set default settings: %d\n",res);

    res = rp_SPI_GetSettings(); // Get uart speed.
    printf("Get current settings of spi: %d\n",res);

    res = rp_SPI_SetMode(RP_SPI_MODE_LIST); // Set SPI mode: Low idle level, sample on trailing edge.
    printf("Set mode: %d\n",res);

    res = rp_SPI_SetSpeed(1000000); // Set SPI speed.
    printf("Set speed: %d\n",res);

    //res = rp_SPI_GetSpeed(1);
    //printf("Get speed: %d\n",res);

    res = rp_SPI_SetWordLen(8); // Set word bit size.
    printf("Set word length: %d\n",res);

    res = rp_SPI_SetSettings(); // Apply settings to SPI.
    printf("Set settings: %d\n",res);

    res = rp_SPI_CreateMessage(1); // Create 2 message.
    printf("Set settings: %d\n",res);

/*    res = rp_SPI_SetBufferForMessage(0,(uint32_t*)buffer,true,3,false); // Set buffer for first message and create RX buffer.
    printf("Set buffers for first msg: %d\n",res);
*/

	return res;
}


int main(int argc, char *argv[]){

    char *buffer = "GREG"; //gregs data
    
	
	char rx_buf[255];
    memset(rx_buf,0,255);
	
	int res = init_SPI(); // gregs code

	// ahe test	
	int data[3]={0x00,0x00,0xff};
	write(res,data,3);

	res = rp_SPI_DestoryMessage();

    res = rp_SPI_Release(); // Close spi api.
    printf("UnInit result: %d\n",res);

    return 0;
}
