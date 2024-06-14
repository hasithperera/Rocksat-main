// Author: Hasith Perera
// email: aep00010@mix.wvu.edu
// RocksatX-WVU 2024 

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "rp_hw-profiles.h"
#include "rp.h"

//#define EPS 0.05
//#define EPS_F 100

#include <time.h>


#define EPS 0.05
#define EPS_F 100

#define debug 1

int data[3];
char *startup_log = "/opt/data/logs/StartUpData.log";
char *event_log = "/opt/data/logs/run.log";
char *data_location = "/opt/data/out/";


//#include "ahe.c"
#include "states.c"


int main(){
	
	#ifdef debug
		struct timespec start, end;
    	long long elapsed_ns;
	#endif	

	log_event("[i] RocksatX-WVU Power up.\n",event_log);

	uint32_t buff_size = 16384; 
	float *buff = (float *)malloc(buff_size * sizeof(float));
	
	startup_count(data,startup_log);
	init_io();
	init_SPI();
	DAC_set(0x8000); // set 0V at startup
		
	//wait for TE to go high
	log_event("[i] Witing for TE.\n",event_log);

	rp_pinState_t TE_state=RP_LOW;
		
	//if time event is not detected the script will hang here
	
	while(TE_state==RP_LOW){
		usleep(10000);
		rp_DpinGetState(RP_DIO3_P,&TE_state);
	}
	
	log_event("[i] Got TE.\n",event_log);
	
	// Antenna extension
	ant_extend();
	log_event("[i] ANT start\n",event_log);
	IterDeployTime(data,startup_log,29);	
	ant_stop();
	log_event("[i] ANT stop\n",event_log);
	
	// Main exp and data saving

	read_state(data,startup_log);
	
	#ifdef debug
		printf("[d] file exp:%d\n",data[2]);
	#endif

	for(int exp_time=data[2];exp_time<230;exp_time++){
		
		#ifdef debug
			printf("[i] exp_itr:%d\n",exp_time);
			clock_gettime(CLOCK_REALTIME,&start);
		#endif		
		
		//	LP and all experiments need to go here
		
		//  LP sweep : exp 1
		RF2_init();
		lp_sweep();
    	rp_AcqGetOldestDataV(RP_CH_2, &buff_size, buff);
		save_data(data[1],data[2],1,data_location,buff,buff_size);
		
		// Sounding experiment
		usleep(300000);
		
		// Its possible to run an additional sweep
		RF2_init();
		lp_sweep();
		rp_AcqGetOldestDataV(RP_CH_2, &buff_size, buff);
		save_data(data[1],data[2],2,data_location,buff,buff_size);


		// LP sweep : exp 3
		RF1_init();
		usleep(150000);
	
		// exp counter
		data[2]++;
		write_state(data,startup_log);
		
		#ifdef debug
			clock_gettime(CLOCK_REALTIME,&end);
    		elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    		printf("[d] Elapsed time: %f ms\n", elapsed_ns/1000000.0);
		#endif
		
	}


	return 0;
}
