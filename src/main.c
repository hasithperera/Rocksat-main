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

//#include <time.h>

#include "ahe.c"
#include "states.c"

#define debug 1

int data[3];
char *startup_log = "/opt/data/logs/StartUpData.log";


int main(){

	
	startup_count(data,startup_log);
	init_io();	
	//wait for TE to go high

	#ifdef debug
		printf("[i]\t\tWiting for TE\n");
	#endif

	rp_pinState_t TE_state=RP_LOW;
	while(TE_state==RP_LOW){
		usleep(10000);
		rp_DpinGetState(RP_DIO3_P,&TE_state);
	}
	#ifdef debug
		printf("[i]\t\tGot TE\n");
	#endif

	// Antenna extension
	ant_extend();
	IterDeployTime(data,startup_log,36);	
	ant_stop();

	// Main data saving
	
	read_state(data,startup_log);

	for(int exp_time=data[3];exp_time<230;exp_time++){
		
		#ifdef debug
			printf("[i]\t\t exp_itr:%d\n",exp_time);
		#endif		


		//LP and all experiments need to go here
		sleep(1);
	

		data[2]++;
		write_state(data,startup_log);

	}


	return 0;
}
