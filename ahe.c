#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "rp_hw-profiles.h"
#include "rp.h"


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
