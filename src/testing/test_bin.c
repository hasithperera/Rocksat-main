#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "rp.h"
//#include "ahe.c"

#define EPS 0.05
#define EPS_F 100


// needs to be before state.c include
char *event_log = "/opt/data/logs/run.log";

#include "states.c"


int lp_sweep_HS(){
	struct timespec req, rem;	
	rp_AcqStart();
	

	uint16_t outputCode = 0x8000;
	rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
	
    DAC_set(outputCode);
	usleep(2000);
	req.tv_sec = 0;
	req.tv_nsec = 200;

	for(int i=0;i<800;i++){
    	DAC_set(0x8000);	
		nanosleep(&req, &rem);
    	DAC_set(0xffff);	
		//nanosleep(&req, &rem);
	}
	
    DAC_set(0x8000); // set 0V
   /* usleep(1);
	usleep(5000);
	DAC_set(0x8000); // set 0V
	

	outputCode = 0x8000;
	for(int i=0;i<800;i++){
    	DAC_set(outputCode-=20);	
		usleep(1);
	}

    DAC_set(0x8000); // set 0V
	*/

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
	
	read_state(data,startup_log); // load exp id from file
	
		
	float freq;
	rp_AcqGetSamplingRateHz(&freq);
	printf("sample freq:%f\n",freq);
	printf("ADC buffer:%d",ADC_BUFFER_SIZE);
	printf("[d] RF1 init: ");
	//RF1_init();
	RF2_init();
	printf("ok\n");
	data[1]=100;

	clock_gettime(CLOCK_REALTIME,&start);

	RF1_init();
	sounding_sweep(3,buff,buff_size,file,data);	
	sounding_ant_flip();
	sounding_sweep(4,buff,buff_size,file,data);	
	sounding_ant_org();
	sounding_sweep(3,buff,buff_size,file,data);	
	sounding_ant_flip();
	sounding_sweep(4,buff,buff_size,file,data);	
	sounding_ant_org();
	

	//event needs timing
	//lp_sweep();
	//DAC_set(0xFFFF-1); // set 0V at startup
    //rp_AcqGetOldestDataV(RP_CH_2, &buff_size, buff);
	//save_data(1,data[2]++,2,file,buff,buff_size); // LP


	
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
