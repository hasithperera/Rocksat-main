#include <stdio.h>
#include <time.h>
#include <string.h>


//#include "ahe.c"

int data[3];
char *startup_log = "/opt/data/logs/StartUpData.log";
char *event_log = "/opt/data/logs/run.log";

#include "states.c"


int main(){

	log_event("[i] Clean startup log.(pre launch)\n",event_log);
	read_state(data,startup_log);
	// clear Deploy time and exp time
	data[0] = 0;
	data[2] = 0;
	write_state(data,startup_log);
	
	return 0;

}
