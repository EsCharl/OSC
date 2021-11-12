#include <stdio.h>

#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif

struct Job
{
    char job_name[20];
    int arrivalTime;
    int burst;
    int burstLeft;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
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
      printf(" 1. Manually Input.\n 2. Input via file. (please ensure the file is in .txt file and only consist of ' , ' to seperate each category. the order of seperation is \"Job name(no spaces)\" , \"Arrival time (please use '.' instead of ':' in seconds)\" , \"Burst (Time needed for the job)(please use '.' instead of ':' in seconds)\")\n");

	    scanf(" %c", &inputMode);
  }

  if (inputMode == '1'){
    while (exit != 'q'){
      printf("Job name?(no spaces)\n");
      scanf("%s", N->job_name);

      printf("Arrival Time?(please use '.' instead of ':')\n");
      scanf("%d", &N->arrivalTime);

      printf("Burst (time needed for the job)\n");
      scanf("%d", &N->burst);

      N->completionTime = 0;
      N->turnaroundTime = 0;
      N->waitingTime = 0;
      N->burstLeft = 0;

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

        sscanf(readline, "%s , %d , %d", N->job_name, &N->arrivalTime, &N->burst);

        N->completionTime = 0;
        N->turnaroundTime = 0;
        N->waitingTime = 0;
        N->burstLeft = 0;

        if(!feof(read)){
          N->next = malloc(sizeof(node));
          N = N->next;
        } // end if
      } // end while

      fclose(read);
    } // end else
  }

  N->next = NULL;

  //return 0;

  while (!(mode == '1' || mode == '2' || mode == '3')){
      printf("Please enter a value between 1 to 3.\n");
      printf("Please select one of the scheduling algorithm that you wish to use.\n 1. Shortest Job First (SJF) Non-Preemptive Version. \n 2. Round Robin (RR) with Overhead. \n 3. Round Robin (RR) without Overhead.\n");

	    scanf(" %c", &mode);
  }

  int sortFlag,doneFlag,timeFlag,overheadFlag,newHead=0,systemClock=0,timeQuantum,delay = 0,hold,queue;
  char nameHold[100];
  nodePtr temp,count,lPtr=NULL;

  printf("Please input the time quantum.\n");
  scanf("%d", &timeQuantum); //get time quantum

  if (mode == '2'){
    printf("Please input the time delay.\n");
    scanf("%d", &delay); //get overhead
  }

  if (sPtr == NULL){ //if list is empty, end program
        printf("No processes found\n");
        return 0;
    }

    do{ //sort processes according to arrival time
        sortFlag = 0; //sort flag
        temp = sPtr;

        while (temp->next != lPtr){

            if (temp->arrivalTime > (temp->next)->arrivalTime){

                strcpy(nameHold,temp->job_name);
                strcpy(temp->job_name,(temp->next)->job_name);
                strcpy((temp->next)->job_name,nameHold); //bubble sort job name
                hold = temp->arrivalTime;
                temp->arrivalTime = (temp->next)->arrivalTime;
                (temp->next)->arrivalTime = hold; //bubble sort arrival time
                hold = temp->burst;
                temp->burst = (temp->next)->burst;
                (temp->next)->burst = hold; //bubble sort burst time
                sortFlag = 1;
            }
            temp = temp->next;
        }
        lPtr = temp;
    }while (sortFlag);

    for(temp=sPtr;temp!=NULL;temp=temp->next){ //copy burst time to burst tie left
        temp->burstLeft = temp->burst;
    }

    do{
        doneFlag = 0; //done flag
        timeFlag = 1; //time flag
        temp = sPtr; //reset pointer

        while(temp!=NULL){

            if(temp->arrivalTime > systemClock){ //if the process has not arrived yet

                if(temp==sPtr){ //if the process is first process
                    systemClock = temp->arrivalTime; //change clock time to arrival time
                }
                else{ //if not first process

                    if(timeFlag){ //time flag checks to see if all previous processes have completed
                        systemClock = temp->arrivalTime; //if all previous processes have completed, change clock time to arrival time
                        newHead = 1; //set new head flag
                    }
                    else{ //if previous processes have not completed, restart loop to complete the previous processes
                        break;
                    }
                }
            doneFlag = 1; // set flag to continue loop
        }
        else if(temp->arrivalTime <= systemClock){ //if the process has arrived

            if(temp->arrivalTime < systemClock - timeQuantum - delay){ //if process arrived before next process starts

                if(temp->burstLeft != 0){ //if there is burst time left

                    if(temp->burstLeft > timeQuantum){ //if burst time greater than time quantum
                        systemClock += (timeQuantum + delay); //add time quantum and delay to clock
                        temp->burstLeft -= timeQuantum; //minus time quantum from burst time
                        timeFlag = 0; //set time flag to indicate previous process has not completed
                    }
                    else{ //if burst time is less than or equal to time quantum
                        systemClock += (temp->burstLeft + delay); //add burst time and delay to clock
                        temp->burstLeft = 0; //set burst time left to 0

                        overheadFlag = 1; //set overhead flag

                        for(count=sPtr;count!=NULL;count=count->next){ //loop through every process
                            if(!((count->arrivalTime <= systemClock - delay&&count->burstLeft == 0)||(count->arrivalTime > systemClock - delay&&count->burstLeft != 0))){
                                overheadFlag = 0; //turn off overhead flag
                                break; //leave loop if even one condition failed
                            }
                        }

                        if(overheadFlag){ //if process has stopped
                            systemClock -= delay; //remove an overhead when all processes before system clock are complete
                        }

                        temp->completionTime = systemClock; //get completion time from clock
                        temp->turnaroundTime = (temp->completionTime - temp->arrivalTime); //get turnaround time from formula
                        temp->waitingTime = (temp->turnaroundTime - temp->burst); //get waiting time from formula
                    }

                    doneFlag = 1; //set flag to continue loop
                    temp = temp->next; //point to next process

                }
                else if(temp->burstLeft == 0){ //if current process has already finished
                    temp = temp->next; //point to next process
                }
            }
            else{ //if process arrived after next process starts
                    queue = 0; //reset queue

                    for(count=temp;count!=NULL;count=count->next){ //for rest of the processes
                        if(count->arrivalTime<=systemClock){ //find which processes arrive within the time quantum and delay
                            queue++; //add number of queue
                        }
                    }

                    while(queue){ //while queue exists

                        if(temp->burstLeft != 0){ //if there is burst time left

                            if(temp->burstLeft > timeQuantum){ //if burst time greater than time quantum
                                systemClock += (timeQuantum + delay); //add time quantum and delay to clock
                                temp->burstLeft -= timeQuantum; //minus time quantum from burst time
                                timeFlag = 0; //set time flag to indicate previous process has not completed
                            }
                            else{ //if burst time is less than or equal to time quantum
                                systemClock += (temp->burstLeft + delay); //add burst time and delay to clock
                                temp->burstLeft = 0; //set burst time left to 0

                                overheadFlag = 1; //set overhead flag

                                for(count=sPtr;count!=NULL;count=count->next){ //loop through every process
                                    if(!((count->arrivalTime <= systemClock - delay&&count->burstLeft == 0)||(count->arrivalTime > systemClock - delay&&count->burstLeft != 0))){
                                        overheadFlag = 0; //turn off overhead flag
                                        break; //leave loop if even one condition failed
                                    }
                                }

                                if(overheadFlag){ //if process has stopped
                                    systemClock -= delay; //remove an overhead when all processes before system clock are complete
                                }

                                temp->completionTime = systemClock; //get completion time from clock
                                temp->turnaroundTime = (temp->completionTime - temp->arrivalTime); //get turnaround time from formula
                                temp->waitingTime = (temp->turnaroundTime - temp->burst); //get waiting time from formula
                            }

                            doneFlag = 1; //set flag to continue loop
                            temp = temp->next; //point to next process

                        }
                        else if(temp->burstLeft == 0){ //if current process has already finished
                            temp = temp->next; //point to next process
                        }

                        queue--; //decrement queue
                    }

                    if((temp==sPtr->next)||newHead){ //if process is first or process has new head of list
                        newHead = 0; //reset head of list flag
                    }
                    else{ //process is not head of list
                        break; //restart loop
                    }
                }
            }
        }
    }while(doneFlag); //flag to continue loop

  for(temp=sPtr;temp!=NULL;temp=temp->next){ //print out all information
    printf("%s , %d , %d : %d %d %d %d\n",temp->job_name,temp->arrivalTime,temp->burst,temp->burstLeft,temp->completionTime,temp->turnaroundTime,temp->waitingTime);
  }

  return 0;

}