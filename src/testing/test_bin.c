#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "ahe.c"
#include "states.c"
#include "rp.h"

#define EPS 0.05
#define EPS_F 100

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
    float average;
	
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
    	DAC_set(outputCode+=20);	
		usleep(1);
	}
	
    DAC_set(0x8000); // set 0V
    usleep(1);
	usleep(5000);
	DAC_set(0x8000); // set 0V
	

	outputCode = 0x8000;
	for(int i=0;i<800;i++){
    	DAC_set(outputCode-=20);	
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


int main(){

	struct timespec start, end;
    long long elapsed_ns;
	
	int data[3];
	char *file="/opt/data/out/";
	char *startup_log = "/opt/data/logs/StartUpData.log";
	

	uint32_t buff_size = 16384; 
	float *buff = (float *)malloc(buff_size * sizeof(float));
	
	init_io();
	init_SPI();
	DAC_set(0x8000); // set 0V at startup
	
	read_state(data,startup_log);
	
		
	float freq;
	rp_AcqGetSamplingRateHz(&freq);
	printf("sample freq:%f\n",freq);
	printf("ADC buffer:%d",ADC_BUFFER_SIZE);
	printf("[d] RF1 init: ");
	//RF1_init();
	RF2_init();
	printf("ok\n");

	clock_gettime(CLOCK_REALTIME,&start);



	//event needs timing
	//get_sounding(buff);
	lp_sweep();
    rp_AcqGetOldestDataV(RP_CH_2, &buff_size, buff);
	save_data(1,data[2]++,2,file,buff,buff_size); // LP

//	save_data(1,data[2]++,1,file,buff,buff_size); // sounding data
	
	clock_gettime(CLOCK_REALTIME,&end);
	
	printf("[i] file id:%05d\n",data[2]-1);	
	write_state(data,startup_log);
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("Elapsed time: %f ms\n", elapsed_ns/1000000.0);

	//printf("pre_trigger cnt:%d\n",pre_trigger_cnt);	
	

	free(buff);
    rp_Release();
	return 0;

}
