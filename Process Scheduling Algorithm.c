#include <stdio.h>

struct Job
{
    char job_name[20];
    int overhead;
    float arrivalTime;
    int burst;
};


int main(void){
    char mode;

    struct Job jobs[50];

    printf("Welcome to the Scheduling Algorithm program!\n");
	

    while (!(mode == '1' || mode == '2' || mode == '3')){
        printf("Please enter a value between 1 to 3.\n");
        printf("Please select one of the scheduling algorithm that you wish to use.\n 1. Shortest Job First (SJF). \n 2. Round Robin (RR) with overhead. \n 3. Round Robin (RR) without overhead.\n");
        
	      scanf(" %c", &mode);
    }
}