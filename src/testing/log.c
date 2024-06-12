
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


int main() {

	struct timespec ts;
	
	clock_gettime(CLOCK_REALTIME,&ts);



    // Print the current time
	//for(int i=0;i<10;I++)
    printf("%ld.%ld\n",ts.tv_sec,ts.tv_nsec);
	long data[2];
 	data[0] = ts.tv_sec;
	data[1]= ts.tv_nsec;
	
    printf("%ld.%ld\n",data[0],data[1]);

	printf("size:%d\n",sizeof(ts.tv_sec));
	printf("size:%d\n",sizeof(data));
    return 0;
}
