
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

uint32_t outputCode;
uint8_t OCBytes[4];
uint8_t tx[3] = {
		0x00,0x00,0x00
	};



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


int DAC_set(uint16_t data){
	// Author: AHE
	// generate specified 24 bit packet
	// bit0: 0
	// bit1 and bit 2: 16 bit data
	
	char data_buffer[3]={0x00,(data&0xff00) >> 8,data&0x00ff};
	
    int res = rp_SPI_CreateMessage(3); // Create 2 message.
 	res = rp_SPI_SetBufferForMessage(0,(uint32_t*)data_buffer,0,3,0);
	return rp_SPI_ReadWrite();
	

}

int main(int argc, char *argv[]){

	int res = init_SPI(); // gregs code

/*	
	uint16_t tmp = 0xf8f1;
	char data[3]={0x00,(tmp&0xff00) >> 8,tmp&0x00ff};
	
    res = rp_SPI_CreateMessage(1); // Create 2 message.
 	res = rp_SPI_SetBufferForMessage(0,(uint32_t*)data,true,3,false);
	res = rp_SPI_ReadWrite();
	//res = rp_SPI_DestoryMessage();
*/	
	
	
	// New DAC syntax

	//DAC_set(0x0ff0);
	//DAC_set(0xffff);


    for(int i = 0; i < 5; i++)
    {
        for(outputCode = 0; outputCode < (uint32_t)65535; outputCode+=128)
        {
	        printf("%2d\t%2X %2X\n", outputCode, OCBytes[1], OCBytes[0]);
	        tx[1]=OCBytes[1];
	        tx[2]=OCBytes[0];
            //printf("%2d\t%2X %2X\n", outputCode, tx[1], tx[2]);
	        //ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
            DAC_set(outputCode);
        }

        for(outputCode = 65280; !exit; outputCode-=255)
        {
	        printf("%2d\t%2X %2X\n", outputCode, OCBytes[1], OCBytes[0]);
	        tx[1]=OCBytes[1];
	        tx[2]=OCBytes[0];
	        //ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	        //printf("%2d\t%2X %2X\n", outputCode, tx[1], tx[2]);
            DAC_set(outputCode);
	        if(outputCode == 0) 
		        break;
        }
	//DAC_set();
    }
	res = rp_SPI_Release(); // Close spi api.
    printf("UnInit result: %d\n",res);

    return 0;
}
