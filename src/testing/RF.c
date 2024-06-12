/* Red Pitaya C API example of Synced Generation and acquisition
on a specific channel */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rp_hw.h"
#include "rp_hw-profiles.h"
#include "rp.h"

#define EPS 0.05
#define EPS_F 100

#include <time.h>
#include "ahe.c"


int RF_avg(float *buff,uint32_t buff_size){
    rp_AcqReset();
    rp_AcqSetDecimation(RP_DEC_2);
    //rp_AcqSetTriggerLevel(RP_CH_1, 0.5);    // Trig level is set in Volts while in SCPI
    rp_AcqSetTriggerDelay(0);

    rp_AcqSetGain(RP_CH_2, RP_LOW);         // user can switch gain using this command

    rp_AcqStart();
    rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
    
	/* After the acquisition is started some time delay is needed to acquire fresh samples into buffer
    Here we have used a time delay of one second but you can calculate the exact value taking into account buffer
    length and sampling rate*/

    usleep(200);

    // !! OS 2.00 or higher only !! //
    bool fillState = false;
    while(!fillState){
        rp_AcqGetBufferFillState(&fillState);
    }
	
    rp_AcqGetOldestDataV(RP_CH_2, &buff_size, buff);
 
	return 0;
}


int RF_avg_int(uint16_t *buff,uint32_t buff_size){

	rp_AcqReset();
    rp_AcqSetDecimation(RP_DEC_2);
    rp_AcqSetTriggerDelay(0);

    rp_AcqSetGain(RP_CH_2, RP_LOW);         // user can switch gain using this command

    rp_AcqStart();

    /* After the acquisition is started some time delay is needed to acquire fresh samples into buffer
    Here we have used a time delay of one second but you can calculate the exact value taking into account buffer
    length and sampling rate*/

    rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
    //rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;
    usleep(200);


    // !! OS 2.00 or higher only !! //
    bool fillState = false;
    while(!fillState){
        rp_AcqGetBufferFillState(&fillState);
    }
	
    rp_AcqGetOldestDataRaw(RP_CH_2, &buff_size, buff);
 
}

int ADC_init(){
	rp_AcqReset();
    rp_AcqSetDecimation(RP_DEC_2);
    //rp_AcqSetTriggerLevel(RP_CH_1, 0.5);    // Trig level is set in Volts while in SCPI
    
	rp_AcqSetTriggerDelay(0xff);

    rp_AcqSetGain(RP_CH_2, RP_LOW);         // user can switch gain using this command


}

int ADC_mod(float *buff,uint32_t buff_size,uint16_t ADC_value){
	
	
	DAC_set(ADC_value); //set 0 V
	rp_AcqStart();
    rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
    
	
	/* After the acquisition is started some time delay is needed to acquire fresh samples into buffer
    Here we have used a time delay of one second but you can calculate the exact value taking into account buffer
    length and sampling rate*/

	// time to get 512 samples
	
    usleep(5);
    rp_AcqGetOldestDataV(RP_CH_2, &buff_size, buff);
 
	return 0;
}



void lp_sweep(){

	// 0 - 10 V 1000 steps
	// SPI only takes 150 ms for this function 
	for(uint16_t outputCode = 0xffff/2; outputCode < 0xffff; outputCode+=0xffff/2000)
    {
            printf("%x\n", outputCode);
            DAC_set(outputCode);
    }



}


int main(int argc, char **argv){
	
	//used to time events
	struct timespec start, end;
    long long elapsed_ns;

    
	/* Print error, if rp_Init() function failed */
    if(rp_Init() != RP_OK){
        fprintf(stderr, "Rp api init failed!\n");
    }

	
    /* Reset Generation and Acquisition */
    //rp_GenReset();
    /* Acquisition */
    uint32_t buff_size = 512;
    float *buff = (float *)malloc(buff_size * sizeof(float));
    float average;	

	//uint16_t *buff_int = (uint16_t *)malloc(buff_size * sizeof(float));
	
	init_SPI();
	ADC_init();	


	clock_gettime(CLOCK_MONOTONIC, &start);	

	//event that needs to be timed

	/*for(int i=0;i<100;i++){
		//printf("%d\n",i);
		RF_avg(buff,buff_size);
	}*/

	// SPI sweep
	//lp_sweep();
	int k = 0;
	for(uint16_t i=0xffff/2;k<255;i+=255){
		ADC_mod(buff,buff_size,i);
		//save_data(k,buff,buff_size);
		k++;
		printf("%d,%d\n",i,k);
	}

	clock_gettime(CLOCK_MONOTONIC, &end);	
	elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
	printf("Elapsed time: %.9f s\n", elapsed_ns/1000000000.0);



	/*
   int i;
    for(i = 0; i < buff_size; i++){
        //printf("%f\n", buff[i]);
	//printf("%d\n",i);
	average += buff[i];
    }
   
    average = average/(i+1);//i + 1 since i starts at 0
    printf("%f\n",average);
    average = 0;
	*/
    
	// Releasing resources
    free(buff);
    rp_Release();
    return 0;
}
