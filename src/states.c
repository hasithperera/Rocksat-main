
// 	My name is Justin Bowman, and I wrote this code
//	June 9th 2024
//	RockSatX WVU
//	jb0123@mix.wvu.edu 

// RPMainCode.c >> states.c
// Renamed and modifed

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>


void IterDeployTime(int *pntr, char *StartUpFileName,int total_delay) {

	for(int i=*(pntr+1); i<=total_delay; i++){
		FILE *fptr;
		*(pntr+1) = *(pntr+1) + 1;
		fptr = fopen(StartUpFileName, "w");
		fprintf(fptr, "%d %s", *(pntr+1), "\n");
		fprintf(fptr, "%d\n", *pntr);
		fprintf(fptr, "%d\n", *(pntr+2)); // ahe-exp time
		fclose(fptr);
		printf("[i]\t\t PWM time:%d\n", i);
		usleep(500000);
	}
}


//ahe -  function to increase experiment count
//call after data is saved

int read_state(int *pntr,char *file){
// read state from file
	FILE *fptr;
	char tmp_data[10];
	int tmp_value;

	fptr = fopen(file, "r");

	// error no file found	
	if(fptr == NULL)
		return 0;
	
	for(int i =0 ;i<3;i++){
		fgets(tmp_data,10,fptr);
		tmp_value =  atoi(tmp_data);
		*(pntr+i) = tmp_value;
	}

	fclose(fptr);	
	return 1; 

}


int write_state(int *pntr,char *file){
//ahe - write state to file
	
	FILE *fptr;
	fptr = fopen(file, "w");
	if(fptr==NULL)
		return 0;
	
	for(int i =0 ;i<3;i++){
		fprintf(fptr,"%d\n",*(pntr+i));
	}
	
	fclose(fptr);	
	return 1;

}

void startup_count(int *pntr, char *StartUpFileName){
	FILE *fptr;
//	int StartUpData
	
	// DeployTime is in half seconds, so if deploytime = 36 it has been deploying for 18 seconds	
	//int TotalDeployTime = 36;	
	int ResetCount = 0;
	int DeployTime = 0;
	int experiment_time = 0;

	

	// Open DeployTime file in read mode
	fptr = fopen(StartUpFileName, "r");

	// Create Space for the content of the file
	char DeployTimeStr[10];
	char ResetCountStr[10];
	char experiment_count_str[10];
	
	// If the StartUp file exists
	if(fptr != NULL) {

		printf("[i]\t\t %s file open ",StartUpFileName);
		// Read the content and print into allocated space
  		fgets(DeployTimeStr, 10, fptr);
		fgets(ResetCountStr, 10, fptr);
		fgets(experiment_count_str, 10, fptr);
		fclose(fptr);

		ResetCount = atoi(ResetCountStr) + 1;
		DeployTime = atoi(DeployTimeStr);
		experiment_time = atoi(experiment_count_str);
	
		fptr = fopen(StartUpFileName, "w");
		fprintf(fptr, "%s", DeployTimeStr);
		fprintf(fptr, "%d\n%d\n", ResetCount,experiment_time);

	// If the file does not exist
	} else {

		printf("[!]\t\t%s file not found!",StartUpFileName);
  		// Open a file in append mode
		fptr = fopen(StartUpFileName, "w");

		// Append a 0 into the time to denote no time has passed for deployment to the file, ie. the antenna are in the stowed position
		fprintf(fptr, "0\n0\n0\n");
	}

	// Close the file
	fclose(fptr);	

	*pntr = ResetCount;
	*(pntr+1) = DeployTime;
	*(pntr+2) = experiment_time; // ahe - added to keep experiment time
}



// Start of Main Code
// uncomment the main function to test the  operation
/*
int main(int argc, char **argv){
	// initialization of DeployTime and ResetCount
	
	int data[2];
	char *StartUpFileName = "StartUpData.txt";
	startup_count(data, StartUpFileName);
	printf("ResetCount, DeployTime :%d,%d\n",data[0],data[1]);
	
	IterDeployTime(data, StartUpFileName);

	return 0;

}
*/












