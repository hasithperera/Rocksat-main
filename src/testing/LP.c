
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
#include "rp.h"
#include "rp_hw-profiles.h"

uint32_t outputCode;

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
 	res = rp_SPI_SetBufferForMessage(0,data_buffer,0,3,0);
	return rp_SPI_ReadWrite();
	

}

int main(int argc, char *argv[]){

	int res = init_SPI(); // gregs code
    for(outputCode = 0x8000; outputCode < 0xffff; outputCode+=1<<6){
		printf("%d\n", outputCode);
        DAC_set(outputCode);
		usleep(100);
    }

    DAC_set(0x8000);
	res = rp_SPI_Release(); // Close spi api.

    return 0;
}

int get_ch0(){
	uint32_t buff_size = 16384;
	float *buff = (float *)malloc(buff_size * sizeof(float));
    float average;
	//printf("%d \n",sizeof(float));
	
	if(rp_Init() != RP_OK){
		fprintf(stderr, "Rp api init failed!\n");
	}
 
	rp_AcqReset();
	rp_AcqSetDecimation(RP_DEC_2);
	//rp_AcqSetTriggerLevel(RP_CH_1, 0);
	//rp_AcqSetTriggerDelay(ADC_BUFFER_SIZE/2.0);
    rp_AcqSetGain(RP_CH_1, RP_LOW);         // user can switch gain using this command

	rp_AcqStart();
	//printf("Acq start\n");
	/* After acquisition is started some time delay is needed in order to acquire fresh samples in to buffer*/
	/* Here we have used time delay of one second but you can calculate exact value taking in to account buffer*/
	/*length and smaling rate*/

    bool fillState = false;
		
	fillState = false;

	//sleep(0.1);

	rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
	
    
	while(!fillState){
		rp_AcqGetBufferFillState(&fillState);
	}

	rp_AcqStop();
			
	rp_AcqGetOldestDataV(RP_CH_1, &buff_size, buff);

    //int i;
    //printf("%f\n", buff[i]);

    
	int i;
    for(i = 0; i < buff_size; i++){

        //printf("%f\n", buff[i]);
	//printf("%d\n",i);
	average += buff[i];
    }
   
    average = average/(i+1);//i + 1 since i starts at 0

    printf("%f\n",average);
    average = 0;

    
}
