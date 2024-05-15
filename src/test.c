

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "rp_hw-profiles.h"
#include "rp.h"

#define EPS 0.05
#define EPS_F 100

#include <time.h>
#include "ahe.c"

// ADC rate
uint32_t getADCRate(){
	uint32_t c = 0;
	if (rp_HPGetBaseFastADCSpeedHz(&c) != RP_HP_OK){
		fprintf(stderr,"[Error] Can't get fast ADC channels count\n");
	}
	return c;
}

int get_ch0(){
	uint32_t buff_size = 16384;
	float *buff = (float *)malloc(buff_size * sizeof(float));
	printf("Start program \n");
	printf("%d \n",sizeof(float));

	//populate the buffer with random numbers
	for(uint32_t i=0;i<buff_size;i++){
		*(buff+i) = ((float)rand()/(float)(RAND_MAX)) * 1.0 -0.5;
		//*(buff+i) = i * 1.00; 
	}
	clock_t start,end = 0;
	double time=0;

	
	printf("init memory done! \n");
	
	start = clock();
	
	char fname[30];
	uint32_t adc_rate;

	adc_rate = getADCRate();
	printf("adc_rate:%d \n",adc_rate);

	printf("[i] test DAC functions\n");
	
	if(rp_Init() != RP_OK){
		fprintf(stderr, "Rp api init failed!\n");
	}
 
	rp_AcqReset();
	rp_AcqSetDecimation(RP_DEC_2);
	rp_AcqSetTriggerLevel(RP_CH_1, 0);
	rp_AcqSetTriggerDelay(ADC_BUFFER_SIZE/2.0);

	int  counter=100;
	bool fillState = false;
		
	fillState = false;
	rp_AcqStart();
	printf("Acq start\n");
	/* After acquisition is started some time delay is needed in order to acquire fresh samples in to buffer*/
	/* Here we have used time delay of one second but you can calculate exact value taking in to account buffer*/
	/*length and smaling rate*/

	sleep(1);
	rp_AcqSetTriggerSrc(RP_TRIG_SRC_CHA_PE);
	rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;
	printf("Acq state 1");
	while(1){
		rp_AcqGetTriggerState(&state);
		if(state == RP_TRIG_STATE_TRIGGERED){
			break;
		}
	}
	printf("Acq got trigger");

	while(!fillState){
		rp_AcqGetBufferFillState(&fillState);
	}

	rp_AcqStop();
				
	start = clock();
	rp_AcqGetOldestDataV(RP_CH_1, &buff_size, buff);
	end = clock();
	
	save_data(1,buff,buff_size,&fname);	
	printf("Acquiring Done\n");
	printf("Total time taken:%.8f\n",(double)(end-start)/CLOCKS_PER_SEC);	


}

// Refactored the code to have a clear main function - ahe
// please add documentation for any changes that are done for this code and add your initials 

// To-Do list of tasks
//  - [ ]  Set RP io for PWM
//  - [ ] get RP_Rx data
//  - [ ] set RP_TX data
//  - [ ] LP DAC
//  - [ ] LP ADC


// set the PWM pins using led blink
// cmd needs to be figured out using the python script`:sq


int init_io(){
	// PWM pins as outputs 
	rp_DpinSetDirection(RP_DIO1_N, RP_OUT);
	rp_DpinSetDirection(RP_DIO2_N, RP_OUT);
	rp_DpinSetDirection(RP_DIO3_N, RP_OUT);
	
	//limit sw testing
	rp_DpinSetDirection(RP_DIO4_N, RP_IN);
	rp_DpinSetDirection(RP_DIO5_N, RP_IN);
	rp_DpinSetDirection(RP_DIO6_N, RP_IN);

}


int main(){

	printf("RP-main for RocksaX");
	init_io();


	return 0;
}


