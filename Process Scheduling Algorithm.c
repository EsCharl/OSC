#include <stdio.h>

struct Job
{
    char job_name[20];
    float arrivalTime;
    float burst;
    struct Job *next;
};

int main(void){
	char fname[100];
  char mode;
  char inputMode;
  char exit;
  char blank;

  int started = 0;

  typedef struct Job node;
	typedef node *nodePtr;

  nodePtr N;
	nodePtr sPtr;
	nodePtr cPtr;

  sPtr = malloc(sizeof(node));

	N = sPtr;
	cPtr = sPtr;

  FILE *read;

  printf("Welcome to the Scheduling Algorithm program!\n");

  printf("How would you like to input the inputs\n");
  while (!(inputMode == '1' || inputMode == '2')){
    printf("Please enter a value of 1 or 2.\n");
      printf(" 1. Manually Input.\n 2. Input via file. (please ensure the file is in .txt file and only consist of ' , ' to seperate each category. the order of seperation is \"Job name(no spaces)\",\"Arrival time (please use '.' instead of ':' in seconds)\",\"Burst (Time needed for the job)(please use '.' instead of ':' in seconds)\")\n");
        
	    scanf(" %c", &inputMode);
  }
    
  if (inputMode == '1'){
    while (exit != 'q'){
      printf("Job name?(no spaces)\n");
      scanf("%s", N->job_name);

      printf("Arrival Time?(please use '.' instead of ':')\n");
      scanf("%f", &N->arrivalTime);

      printf("Burst (time needed for the job)\n");
      scanf("%f", &N->burst);
        
      printf("Would you like to add more jobs? (q to quit)\n");

      scanf("%c", &blank);
      scanf("%c", &exit);

      if (exit != 'q'){
        N->next = malloc(sizeof(node));
        N = N->next;
      }
    }
  }else{
    printf("Please input the file you'd like to load into the program.\n");
	  scanf("%s", fname);
	  read = fopen(fname,"r");

    //if the file is not found.
    if(read == NULL){
		  printf("%s could not be found",fname);
		  sleep(5);
		return 0;
	  }else{
          // start loading it into a linked list.
      while(!feof(read)){
        char readline[100];
        fgets(readline, sizeof(readline),read);

        sscanf(readline, "%s , %f , %f", N->job_name, &N->arrivalTime, &N->burst);

        if(!feof(read)){
          N->next = malloc(sizeof(node));
          N = N->next;
        } // end if
      } // end while

      fclose(read);
    } // end else
  }

  N->next = NULL;
  
  return 0;
  
  while (!(mode == '1' || mode == '2' || mode == '3')){
      printf("Please enter a value between 1 to 3.\n");
      printf("Please select one of the scheduling algorithm that you wish to use.\n 1. Shortest Job First (SJF) Non-Preemptive Version. \n 2. Round Robin (RR) with Overhead. \n 3. Round Robin (RR) without Overhead.\n");
        
	    scanf(" %c", &mode);
  }

  
    
}

