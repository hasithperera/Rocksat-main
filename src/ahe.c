#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "rp_hw-profiles.h"
#include "rp.h"
#include "rp_hw.h"

#define debug_print 1

void test_func(){
	printf("external file");
}
	
	
int save_data(int i,float *buff,uint32_t buff_size){
	char fname[30];	
	FILE *write_ptr;
	sprintf(fname,"out/test-%03d.bin",i);
	write_ptr = fopen(fname,"wb");  // w for write, b for binary
	fwrite(buff,sizeof(float),buff_size,write_ptr); // write 10 bytes from our buffer
	fclose(write_ptr);

	return 0;
}

// set the PWM pins using led blink
// cmd needs to be figured out using the python script


int init_io(){

    // Initialization of API
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }		

	// PWM pins as outputs 
	rp_DpinSetDirection(RP_DIO1_N, RP_OUT);
	rp_DpinSetDirection(RP_DIO2_N, RP_OUT);
	rp_DpinSetDirection(RP_DIO3_N, RP_OUT);

	//TE IN
	rp_DpinSetDirection(RP_DIO3_P, RP_IN);

	//limit sw - tested for operation 
	rp_DpinSetDirection(RP_DIO4_N, RP_IN);
	rp_DpinSetDirection(RP_DIO5_N, RP_IN);
	rp_DpinSetDirection(RP_DIO6_N, RP_IN);

	return 0;
}


int ant_extend(){
	//led 8 bit control register specs 
	// verify this python cmd = 2

	//test52.bin
	//rp_LEDSetState(0x00000002);
	

	//test_Z10_6x.bin
	// Refer memmory map of python_data_in
	rp_LEDSetState(1<<1|1<<0|1<<3|1<<4); 
	
#ifdef debug_print
	printf("[i]\t ant extend\n");
#endif
	return 0;
}

int ant_stop(){

#ifdef debug_print
	printf("[i]\t ant stop\n");
#endif
	uint32_t led_state;

	rp_LEDGetState(&led_state);
	rp_LEDSetState(led_state&~(1|1<<3|1<<4));
	
	return 0;
}

int ant_retract(){
	//engage retraction
	//this will auto stop each servo when internal lim is triggered
	
	rp_LEDSetState(0x00000004);
	return 0;
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


int DAC_set(uint16_t data){
	// Author: AHE
	// generate specified 24 bit packet
	// bit0: 0
	// bit1 and bit 2: 16 bit data
	
	char data_buffer[3]={0x00,(data&0xff00) >> 8,data&0x00ff};
	
    rp_SPI_CreateMessage(3); // Create 2 message.
 	rp_SPI_SetBufferForMessage(0,(uint8_t*)data_buffer,0,3,0);
	return rp_SPI_ReadWrite();
	

}

