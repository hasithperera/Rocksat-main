#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <time.h>


#include "rp_hw-profiles.h"
#include "rp.h"
#include "rp_hw.h"

#define debug_print 1
#define debug 1
void test_func(){
	printf("external file");
}



	
	
int save_data(int restart_id,int time_id,int exp_id,char *file_loc,float *buff,uint32_t buff_size){
	// save a time stamp followed by data
	// to read file created here use time=np.intc and data=np.float32

	char fname[100];	
	FILE *write_ptr;
	sprintf(fname,"%s/%05d_%d_%05d.bin",file_loc,restart_id,exp_id,time_id);
	write_ptr = fopen(fname,"wb");  // w for write, b for binary
	
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME,&ts);
	long time_stamp[2];
	
	
	time_stamp[0]=ts.tv_sec;
	time_stamp[1]=ts.tv_nsec;

	#ifdef debug_print
		printf("time:%ld.%ld\n",time_stamp[0],time_stamp[1]);
	#endif

	fwrite(time_stamp,sizeof(long),2,write_ptr); //time stamp
	fwrite(buff,sizeof(float),buff_size,write_ptr); //write data
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
	rp_Reset();	
	
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
	printf("[i] ant extend\n");
#endif
	return 0;
}


int ant_stop(){

#ifdef debug_print
	printf("[i] ant stop\n");
#endif
	uint32_t led_state;

	rp_LEDGetState(&led_state);
	rp_LEDSetState(led_state&~(1|1<<3|1<<4));
	
	return 0;
}

int rand_TX(){
	//>v52
	return rp_LEDSetState(1<<5); 
}


int rand_off(){
	//>v52
	uint32_t led_state;

	rp_LEDGetState(&led_state);
	return rp_LEDSetState(led_state&~(1<<5)); 
}


int ant_retract(){
	//engage retraction
	//this will auto stop each servo when internal lim is triggered
	//v63 - compatible

	rp_LEDSetState(1<<0|1<<1|1<<2|1<<3|1<<4);
	return 0;
}


int init_SPI(){
	//Author: Greg Lusk
	//notes: code tested for SPI clock speed 
	
    int res = rp_SPI_InitDevice("/dev/spidev1.0"); // Init spi api.
    res = rp_SPI_SetDefaultSettings(); // Set default settings.
    res = rp_SPI_GetSettings(); // Get uart speed.
    res = rp_SPI_SetMode(RP_SPI_MODE_LIST); // Set SPI mode: Low idle level, sample on trailing edge.
    res = rp_SPI_SetSpeed(1000000); // Set SPI speed.
    res = rp_SPI_SetWordLen(8); // Set word bit size.
    res = rp_SPI_SetSettings(); // Apply settings to SPI.
    res = rp_SPI_CreateMessage(1); // Create 2 message.

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

void log_event(char *text,char *event_log){

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME,&ts);
	#ifdef debug
		printf("%ld.%ld\t%s",ts.tv_sec,ts.tv_nsec,text);
	#endif

	FILE *fp=fopen(event_log,"a+");
	fprintf(fp,"%ld.%ld\t%s",ts.tv_sec,ts.tv_nsec,text);
	fclose(fp);
}

// Experiment functions:

int RF1_init(){
// for Sounding mode
	
	rp_AcqReset();
    	rp_AcqSetDecimation(RP_DEC_1);
	

	rp_AcqSetGain(RP_CH_2, RP_LOW);// user can switch gain using this command
    	rp_AcqSetGain(RP_CH_1, RP_LOW);// user can switch gain using this command
 	
	rp_AcqSetTriggerDelay(ADC_BUFFER_SIZE/2.0);
	
	//rp_AcqSetAveraging(1);				
	return 0;   
}

int RF2_init(){
// for LP experiments

	rp_AcqReset();
    	rp_AcqSetDecimation(RP_DEC_2048);

	rp_AcqSetAveraging(1);				
	rp_AcqSetTriggerDelay(ADC_BUFFER_SIZE/2.0);
	return 0;   
}

uint32_t get_sounding_with_TX(float *buff){
// get the full memory bank 2^16 

	uint32_t buff_size = ADC_BUFFER_SIZE;
	printf("[d] Acq start:\n");
	
	rand_TX();
	rp_AcqStart();
	rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
	usleep(150);	
	bool fillState = false;
	while(!fillState){
	      rp_AcqGetBufferFillState(&fillState);
	}
	
	rp_AcqStop();
	rand_off();
    	rp_AcqGetOldestDataV(RP_CH_1, &buff_size, buff);
    
    return buff_size;
}

int lp_sweep(){
	
	rp_AcqStart();
	

	uint16_t outputCode = 0x8000;
	rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
	
    	DAC_set(outputCode);
	usleep(2000);

	for(int i=0;i<800;i++){
    		DAC_set(outputCode+=40);	
		usleep(1);
	}
	
    	DAC_set(0x8000); // set 0V
    	usleep(1);
	usleep(5000);
	DAC_set(0x8000); // set 0V
	

	outputCode = 0x8000;
	for(int i=0;i<800;i++){
    		DAC_set(outputCode-=40);	
		usleep(1);
	}

   	 DAC_set(0x8000); // set 0V
	
	bool fillState = false;
	while(!fillState){
	      rp_AcqGetBufferFillState(&fillState);
	}
	
	rp_AcqStop();
	return 0;
}



