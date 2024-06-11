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

#include "ahe.c"
#include "states.c"

#define debug 1

int data[3];
char *startup_log = "/opt/data/logs/StartUpData.log";
char *event_log = "/opt/data/logs/run.log";


void log_event(char *text){

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME,&ts);
	#ifdef debug
		printf("%ld.%ld\t%s",ts.tv_sec,ts.tv_nsec,text);
	#endif

	FILE *fp=fopen(event_log,"a+");
	fprintf(fp,"%ld.%ld\t%s",ts.tv_sec,ts.tv_nsec,text);
	fclose(fp);
}

int main(){

	
	startup_count(data,startup_log);
	init_io();	
	//wait for TE to go high

	log_event("[i] Witing for TE.\n");

	rp_pinState_t TE_state=RP_LOW;
		
	//if time event is not detected the script will hang here
	
	while(TE_state==RP_LOW){
		usleep(10000);
		rp_DpinGetState(RP_DIO3_P,&TE_state);
	}
	
	log_event("[i] Got TE.\n");
	
	// Antenna extension
	ant_extend();
	IterDeployTime(data,startup_log,36);	
	ant_stop();

	log_event("[i] Deployment done.\n");
	
	// Main data saving

	read_state(data,startup_log);
	
	#ifdef debug
		printf("[d]file exp:%d\n",data[2]);
	#endif

	for(int exp_time=data[2];exp_time<230;exp_time++){
		
		#ifdef debug
			printf("[i]\t\t exp_itr:%d\n",exp_time);
		#endif		


		//LP and all experiments need to go here
		sleep(1);
	
		// exp counter
		data[2]++;
		write_state(data,startup_log);

	}


	return 0;
}
