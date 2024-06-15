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
	
	log_event("[i] Start ant retraction\n",event_log);
	init_io();
	ant_retract();
	usleep(15700000);	
	ant_stop();

	log_event("[i] ant retraction done\n",event_log);

	return 0;
}
