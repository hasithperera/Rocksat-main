#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "rp_hw-profiles.h"
#include "rp.h"

#define debug_print 1

void test_func(){
	printf("external file");
}
	
	
int save_data(int i,float *buff,uint32_t buff_size,char *fname){

	FILE *write_ptr;
	sprintf(fname,"out/test-%03d.bin",i);
	write_ptr = fopen(fname,"wb");  // w for write, b for binary
	fwrite(buff,sizeof(float),buff_size,write_ptr); // write 10 bytes from our buffer
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

	// PWM pins as outputs 
	rp_DpinSetDirection(RP_DIO1_N, RP_OUT);
	rp_DpinSetDirection(RP_DIO2_N, RP_OUT);
	rp_DpinSetDirection(RP_DIO3_N, RP_OUT);
	
	//limit sw - tested for operation 
	rp_DpinSetDirection(RP_DIO4_N, RP_IN);
	rp_DpinSetDirection(RP_DIO5_N, RP_IN);
	rp_DpinSetDirection(RP_DIO6_N, RP_IN);

	return 0;
}


int ant_extend(){
	//led 8 bit control register specs 
	// verify this python cmd = 2
	rp_LEDSetState(0x00000002);
	
#ifdef debug_print
	printf("[i]\t ant extend\n");
#endif
	return 0;
}

int ant_stop(){

#ifdef debug_print
	printf("[i]\t ant stop\n");
#endif

	rp_LEDSetState(0x00000000);
	return 0;
}

int ant_retract(){
	//engage retraction
	//this will auto stop each servo when internal lim is triggered
	
	rp_LEDSetState(0x00000004);
	return 0;
}


