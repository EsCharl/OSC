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
	int waitingTimeSelected;
	int entryTime;
	int loadFlag;
	struct Job* next;
};

struct Set
{
    int set_num;
    int job_num;
    int job_before;
    struct Job* firstJob;
    struct Set* nextSet;
};

int main(void) {
	char fname[100];
	char inputMode;
	char exit;
	char blank;
	char quit;

	char done; //new

	char Jobs[6][60] = { "Shortest Job First (SJF) Preemptive Version", "Shortest Job First (SJF) Non-Preemptive Version", "Round Robin (RR) with Overhead", "Round Robin (RR) without Overhead","Round Robin (RR) with Overhead without Arrival Time", "Round Robin (RR) without Overhead without Arrival Time" };

	int started = 0;
	int mode;
	int fail = 0;

	char checkIfInt[10];

	typedef struct Job node;
	typedef node* nodePtr;

	typedef struct Set set; //new
	typedef set* setPtr; //new

	nodePtr N;
	nodePtr cleaner;

	setPtr S; //new
	setPtr fPtr; //new
	setPtr setCleaner; //new

	FILE* read;

	node* save; //array of structs
	nodePtr* queue; //array of pointers

	do {

        int totalJobs = 0;

        int setNum = 0; //new

        fPtr = malloc(sizeof(set)); //new

        S = fPtr; //new

        printf("Welcome to the Scheduling Algorithm program!\n");

	do {

		int jobNum = 0;

		N = malloc(sizeof(node));

		S->firstJob = N;
		S->job_before = totalJobs;

		printf("\nHow would you like to input the inputs\n");
		do {
			printf("Please enter a value of 1 or 2.\n");
			printf(" 1. Manually Input.\n 2. Input via .txt file.\n");

			scanf(" %c", &inputMode);
		} while (!(inputMode == '1' || inputMode == '2'));

		scanf("%c", &blank);

		if (inputMode == '1') {
			do {
				do {
					fail = 0;
					printf("\nJob name?(no spaces)\n");
					gets(N->job_name);

					for (int x = 0; x < strlen(N->job_name); x++) {
						if (N->job_name[x] == ' ') {
							fail++;
						}
					}
				} while (fail != 0 || strlen(N->job_name) == 0);

				printf("\n");

				do {
					fail = 0;
					printf("Arrival Time? (Integer)\n");
					gets(checkIfInt);

					for (int x = 0; x < strlen(checkIfInt); x++) {
						if (checkIfInt[x] - '0' > 9 || checkIfInt[x] - '0' < 0) {
							fail++;
						}
					}
				} while (fail != 0 || strlen(checkIfInt) == 0 || atoi(checkIfInt) == 0);

				N->arrivalTime = atoi(checkIfInt);
				printf("\n");

				do {
					fail = 0;
					printf("Burst Time (integer)\n");
					gets(checkIfInt);

					for (int x = 0; x < strlen(checkIfInt); x++) {
						if (checkIfInt[x] - '0' > 9 || checkIfInt[x] - '0' < 0) {
							fail++;
						}
					}
				} while (fail != 0 || strlen(checkIfInt) == 0 || atoi(checkIfInt) == 0);

				N->burst = atoi(checkIfInt);
				printf("\n");

				N->completionTime = 0;
				N->turnaroundTime = 0;
				N->waitingTime = 0;
				N->waitingTimeSelected = 0;
				N->burstLeft = 0;
				N->entryTime = 0;
				N->loadFlag = 0;

				jobNum++;

				printf("Would you like to add more jobs? (q to quit)\n");

				scanf("%c", &exit);

				if (exit != 'q' || jobNum <= 1) {
					N->next = malloc(sizeof(node));
					N = N->next;
				}

				if (exit == 'q' && jobNum == 1) {
					printf("this software need at least 2 jobs\n");
				}

				scanf("%c", &blank);

			} while (exit != 'q' || jobNum <= 1);
		}
		else {
			printf("\nPlease input the file you'd like to load into the program.\n");
			printf("    *Please ensure the file is in .txt file\n    *Use \" , \" to seperate each category.the format of seperation should be : \n(Job Name) , (Arrival time) , (Burst)\n\n * Job Name Should not contain any spaces\n");
			scanf("%s", fname);
			read = fopen(fname, "r");

			//if the file is not found.
			if (read == NULL) {
				printf("%s could not be found", fname);
				sleep(5);
				return 0;
			}
			else {
				// start loading it into a linked list.
				while (!feof(read)) {
					char readline[100];
					fgets(readline, sizeof(readline), read);

					sscanf(readline, "%s , %d , %d", N->job_name, &N->arrivalTime, &N->burst);

					N->completionTime = 0;
					N->turnaroundTime = 0;
					N->waitingTime = 0;
					N->waitingTimeSelected = 0;
					N->burstLeft = 0;
					N->entryTime = 0;
					N->loadFlag = 0;
					jobNum++;

					if (!feof(read)) {
						N->next = malloc(sizeof(node));
						N = N->next;
					} // end if
				} // end while

				fclose(read);
				if (jobNum <= 1) {
					printf("the text file need at least 2 jobs\n");
					printf("the software will now shut down");
					printf("\n");
					sleep(5);
					return 0;
				}
				scanf("%c", &blank);

			} // end else

		}
            N->next = NULL;
            printf("\n%i jobs entered.\n", jobNum);

            S->job_num = jobNum;
            S->set_num = setNum;
            totalJobs += jobNum;
            setNum++;

            printf("Would you like to add more sets? (q to quit)\n");

            scanf("%c", &done);

            if (done != 'q' || setNum <= 1) {
                S->nextSet = malloc(sizeof(set));
                S = S->nextSet;
            }

            if (done == 'q' && setNum == 1) {
                printf("this software need at least 2 sets\n");
            }

            scanf("%c", &blank);

        } while (done != 'q' || setNum <= 1);

        S->nextSet = NULL;
        printf("\n%i sets entered.\n", setNum);

		//return 0;

		int sortFlag, systemClock = 0, hold, count = 0, breakFlag = 1, loop, lockFlag, size = 0, delayFlag = 0, delayTime, delay = 0, timeLeft, timeQuantum = 0, nullFlag = 0;
		char nameHold[100];

		do {
			fail = 0;
			printf("\nPlease input the time quantum. (Integer)\n");
			gets(checkIfInt);

			for (int x = 0; x < strlen(checkIfInt); x++) {
				if (checkIfInt[x] - '0' > 9 || checkIfInt[x] - '0' < 0) {
					fail++;
				}
			}
		} while (fail != 0 || strlen(checkIfInt) == 0 || atoi(checkIfInt) == 0);

		timeQuantum = atoi(checkIfInt);

		do { //make sure input is not 0
			fail = 0;
			printf("\nPlease input the time delay. (Integer)\n");
			gets(checkIfInt);

			for (int x = 0; x < strlen(checkIfInt); x++) {
				if (checkIfInt[x] - '0' > 9 || checkIfInt[x] - '0' < 0) {
					fail++;
				}
			}
		} while (fail != 0 || strlen(checkIfInt) == 0 || atoi(checkIfInt) == 0);

		delay = atoi(checkIfInt);

		nodePtr temp, lPtr = NULL, compare;
        setPtr tempSet;

		if (fPtr->firstJob == NULL) { //if list is empty, end program
			printf("No processes found\n");
			return 0;
		}

		printf("\n");

		for (tempSet = fPtr; tempSet != NULL; tempSet = tempSet->nextSet) { //get size of list
			if(tempSet->job_num > size){
                size = tempSet->job_num ;
			}
		}

		queue = malloc(sizeof(*queue) * (size + 2)); //get space for queue

		save = malloc(sizeof(node) * 6 * totalJobs); //get space to save array results, 6 is for 6 algorithms

		double avgWaitTime[6*setNum], avgTurnaroundTime[6*setNum];

		for (mode = 0; mode < 6; mode++) { //loop through all modes

            tempSet = fPtr;
            printf("\n%s\n\n", Jobs[mode]);

            do{

			//reset variables
			for (temp = tempSet->firstJob; temp != NULL; temp = temp->next) { //loop through list
				temp->burstLeft = temp->burst; //copy burst time to burst time left
				temp->waitingTimeSelected = 0; //reset first time processing flag
				temp->loadFlag = 0; //reset load flag
			}

			if (mode == 4 || mode == 5) { //for RR without arrival time

				for (temp = tempSet->firstJob; temp != NULL; temp = temp->next) { //set all arrival time = 0
					temp->arrivalTime = 0;
				}
			}

			for (loop = 0; loop < (size + 2); loop++) { //get two more extra space for queueing and sorting
				queue[loop] = NULL; //set null
			}

			if (mode == 2 || mode == 4) {

				timeLeft = timeQuantum; //set initial time
				delayTime = delay; //set delay time
			}
			else if (mode == 3 || mode == 5) {

				timeLeft = timeQuantum; //set initial time
			}

			for (systemClock = 0; breakFlag; systemClock++) { //system clock, ends when processes terminate

				for (temp = tempSet->firstJob; temp != NULL; temp = temp->next) { //loop through list

					if (temp->arrivalTime == systemClock && temp->burstLeft != 0) { //if arrival time matches clock and burst time left

						if (queue[0] == NULL) { //if first node is empty
							queue[0] = temp; //fill first node
							lockFlag = 1; //to stop same loop operation
						}
						else { //if first node is filled

							for (loop = 0; loop < tempSet->job_num; loop++) { //loop through queue

								if (queue[loop] == NULL) { //find empty node
									queue[loop] = temp; //fill empty node
									break;
								}
							}
						}
					}
				}

				if (delayFlag && (mode == 2 || mode == 4)) { //delay flag set

					if (delayTime == delay) {
						printf("-> %d [DELAY] ", systemClock - 1);
					}

					delayTime--; //minus delay time

					if (delayTime == 0) { //if delay time finished
						printf("%d ", systemClock);
						delayFlag = 0; //turn off delay flag
					}

					continue; //go to next loop without processing
				}

				if (mode == 1) { //non-preemptive

					do { //sort processes according to burst time left
						sortFlag = 0; //sort flag
						loop = 1; //start from second node

						while (queue[loop + 1] != lPtr) {

							if (queue[loop]->burstLeft > queue[loop + 1]->burstLeft) {

								queue[size] = queue[loop];
								queue[loop] = queue[loop + 1];
								queue[loop + 1] = queue[size]; //bubble sort queue pointer
								queue[size] = NULL; //set temp pointer to NULL

								sortFlag = 1;
							}
							loop++;
						}
						lPtr = queue[loop];
					} while (sortFlag);

					lPtr = NULL; //reset pointer
				}

				if (queue[0] != NULL && !(lockFlag)) { //if process is in queue and not same loop

					if (nullFlag == 1) { //close null loop
						printf("%d ", systemClock - 1);
						nullFlag = 0;
					}

					if (queue[0]->loadFlag == 0) {
						printf("-> %d [%s", systemClock - 1, queue[0]->job_name); //front part of queue
						queue[0]->loadFlag = 1;
					}

					if (mode == 2 || mode == 3 || mode == 4 || mode == 5) {
						timeLeft--; //minus time
					}

					queue[0]->burstLeft--; //minus burst

					if (queue[0]->waitingTimeSelected == 0) { //flag to check first time enter queue
						printf("*"); //star to show first insertion
						queue[0]->waitingTimeSelected = 1; //set flag
						queue[0]->waitingTime = (systemClock - 1) - queue[0]->arrivalTime; //systemClock - 1 because process is only processed on the next second
						queue[0]->entryTime = systemClock - 1;
					}

					if (queue[0]->burstLeft == 0) { //if process completes

						if (queue[0]->loadFlag == 1) {
							printf("(%d/%d)*] %d ", (queue[0]->burst - queue[0]->burstLeft), queue[0]->burst, systemClock); //second part of queue
							queue[0]->loadFlag = 0;
						}

						queue[0]->completionTime = systemClock; //get completion time from clock
						queue[0]->turnaroundTime = (queue[0]->completionTime - queue[0]->arrivalTime); //get turnaround time from formula//get waiting time from formula

						for (loop = 0; loop < tempSet->job_num; loop++) { //loop through queue
							queue[loop] = queue[loop + 1]; //move jobs to the front
							queue[loop + 1] = NULL; //set last queue as NULL
						}

						if (mode == 2 || mode == 3 || mode == 4 || mode == 5) {
							timeLeft = timeQuantum; //reset time
						}

						if (queue[0] != NULL && (mode == 2 || mode == 4)) { //if there is still a job and correct mode
							delayFlag = 1; //set delay flag
							delayTime = delay; //reset delay time
						}
					}
					else if (timeLeft == 0 && (mode == 2 || mode == 3 || mode == 4 || mode == 5)) { //if time finishes

						if (queue[0]->loadFlag == 1) {
							printf("(%d/%d)] %d ", (queue[0]->burst - queue[0]->burstLeft), queue[0]->burst, systemClock); //second part of queue
							queue[0]->loadFlag = 0;
						}

						for (loop = 0; loop < (tempSet->job_num + 1); loop++) { //loop through queue

							if (queue[loop] == NULL) { //find empty node
								queue[loop] = queue[0]; //fill empty node
								break;
							}
						}

						for (loop = 0; loop < tempSet->job_num; loop++) { //loop through queue
							queue[loop] = queue[loop + 1]; //move jobs to the front
							queue[loop + 1] = NULL; //set last queue as NULL
						}

						if (mode == 2 || mode == 3 || mode == 4 || mode == 5) {
							timeLeft = timeQuantum; //reset time
						}

						if (queue[0] != NULL && (mode == 2 || mode == 4)) { //if there is still a job and correct mode
							delayFlag = 1; //set delay flag
							delayTime = delay; //reset delay time
						}
					}
				}
				else if ((queue[0] == NULL || (queue[0] != NULL && lockFlag)) && systemClock != 0) {

					if (nullFlag == 0) {
						printf("-> %d [NULL] ", systemClock - 1);
						nullFlag = 1;
					}
				}

				if (lockFlag) { //prevent same loop running
					lockFlag = 0;
				}

				if (mode == 0) { //preemptive

					do { //sort processes according to burst time left
						sortFlag = 0; //sort flag
						loop = 0; //start from first node

						while (queue[loop + 1] != lPtr) {

							if (queue[loop]->burstLeft > queue[loop + 1]->burstLeft) {

								if (queue[0]->loadFlag == 1 && queue[0]->burstLeft > queue[1]->burstLeft) {
									printf("(%d/%d)] %d ", (queue[0]->burst - queue[0]->burstLeft), queue[0]->burst, systemClock); //second part of queue
									queue[0]->loadFlag = 0;
								}

								queue[size] = queue[loop];
								queue[loop] = queue[loop + 1];
								queue[loop + 1] = queue[size]; //bubble sort queue pointer
								queue[size] = NULL; //set temp pointer to NULL

								sortFlag = 1;
							}
							loop++;
						}
						lPtr = queue[loop];
					} while (sortFlag);

					lPtr = NULL; //reset pointer
				}

				breakFlag = 0; //break flag

				for (temp = tempSet->firstJob; temp != NULL; temp = temp->next) { //if all processes ended
					if (temp->burstLeft != 0) {
						breakFlag = 1; //set flag
						break;
					}
				}
			}

			breakFlag = 1; //reset break flag;

			for (loop = 0; loop < (size + 2); loop++) { //reset queue
				queue[loop] = NULL; //set null
			}

			temp = tempSet->firstJob; //reset pointer

			for (loop = 0; loop < tempSet->job_num; loop++) {

				queue[loop] = temp;
				temp = temp->next;
			}

			do { //sort processes according to completion time
				sortFlag = 0; //sort flag
				loop = 0;

				while (queue[loop + 1] != lPtr) {

					if (queue[loop]->completionTime > queue[loop + 1]->completionTime) {

						queue[size] = queue[loop];
						queue[loop] = queue[loop + 1];
						queue[loop + 1] = queue[size]; //bubble sort queue pointer
						queue[size] = NULL; //set temp pointer to NULL

						sortFlag = 1;
					}
					loop++;
				}
				lPtr = queue[loop];
			} while (sortFlag);

			lPtr = NULL; //reset pointer

			hold = 0; //temp

			for (loop = (totalJobs*mode) + tempSet->job_before; loop < (totalJobs*mode) + tempSet->job_before + tempSet->job_num; loop++) { //loop through save array

				strcpy(save[loop].job_name, queue[hold]->job_name);
				save[loop].burst = queue[hold]->burst;
				save[loop].completionTime = queue[hold]->completionTime;
				save[loop].turnaroundTime = queue[hold]->turnaroundTime;
				save[loop].waitingTime = queue[hold]->waitingTime;
				save[loop].arrivalTime = queue[hold]->arrivalTime;
				save[loop].entryTime = queue[hold]->entryTime; //save using queue

				hold++;
			}

			for (loop = 0; loop < (size + 2); loop++) { //reset queue
				queue[loop] = NULL; //set null
			}

			for (temp = tempSet->firstJob; temp != NULL; temp = temp->next) { //reset statistics

				temp->completionTime = 0;
				temp->turnaroundTime = 0;
				temp->waitingTime = 0;
				temp->entryTime = 0;
			}

			int sumWaitTime = 0, sumTurnaroundTime = 0;

			printf("\n%-15s | %-3s | %-3s : %-6s | %-6s | %-6s | %-6s\n", "Job Name", "A.T", "B.T", "E.T", "C.T", "T.A.T", "W.T");

			for (loop = (totalJobs*mode) + tempSet->job_before; loop < (totalJobs*mode) + tempSet->job_before + tempSet->job_num; loop++) { //print out information from loop

				printf("%-15s | %-3d | %-3d : %-6d | %-6d | %-6d | %-6d\n", save[loop].job_name, save[loop].arrivalTime, save[loop].burst, save[loop].entryTime, save[loop].completionTime, save[loop].turnaroundTime, save[loop].waitingTime);

				sumWaitTime += save[loop].waitingTime;
				sumTurnaroundTime += save[loop].turnaroundTime;
			}

			avgWaitTime[mode+(tempSet->set_num*6)] = sumWaitTime / (double)tempSet->job_num;
			avgTurnaroundTime[mode+(tempSet->set_num*6)] = sumTurnaroundTime / (double)tempSet->job_num;

			printf("Average Turnaround time: %0.3f | Average Waiting Time: %0.3f\n\n", avgTurnaroundTime[mode+(tempSet->set_num*6)], avgWaitTime[mode+(tempSet->set_num*6)]);

			tempSet = tempSet->nextSet;

			}while(tempSet!=NULL);

		}
		printf("\nA.T = Arrival Time\nB.T = Burst Time\nE.T = Entry Time\nC.T = Complete Time\nT.A.T = Turnaround Time\nW.T = Wait Time\n");

		tempSet = fPtr;

		do{

            double lowestAvgWaitingTime = 999999999999999999, lowestAvgTurnaroundTime = 999999999999999999;
            int lowestAvgWaitingTimeIndex, lowestAvgTurnaroundTimeIndex;
            for (int i = 0; i < 6; i++) {
                if (lowestAvgWaitingTime >= avgWaitTime[i+(tempSet->set_num*6)]) {
                    lowestAvgWaitingTime = avgWaitTime[i+(tempSet->set_num*6)];
                    lowestAvgWaitingTimeIndex = i+(tempSet->set_num*6);
                }

                if (lowestAvgTurnaroundTime >= avgTurnaroundTime[i+(tempSet->set_num*6)]) {
                    lowestAvgTurnaroundTime = avgTurnaroundTime[i+(tempSet->set_num*6)];
                    lowestAvgTurnaroundTimeIndex = i+(tempSet->set_num*6);
                }
            }

            printf("\n");

            printf("\nLowest Avg Turnaround Time For Set %d : %0.3f\n", tempSet->set_num+1, avgTurnaroundTime[lowestAvgTurnaroundTimeIndex]);
            for (int i = 0; i < 6; i++) {
                if (avgTurnaroundTime[i+(tempSet->set_num*6)] == avgTurnaroundTime[lowestAvgTurnaroundTimeIndex])
                    printf("> %s\n", Jobs[i]);
            }

            printf("\nLowest Avg Waiting Time For Set %d : %0.3f\n", tempSet->set_num+1, avgWaitTime[lowestAvgWaitingTimeIndex]);
            for (int i = 0; i < 6; i++) {
                if (avgWaitTime[i+(tempSet->set_num*6)] == avgWaitTime[lowestAvgWaitingTimeIndex])
                    printf("> %s\n", Jobs[i]);
            }

            tempSet = tempSet->nextSet;

		}while(tempSet != NULL);

		printf("\nto quit : 'q'\nto restart : input any key\n");

		scanf(" %c", &quit);

		scanf("%c", &blank);

		printf("\n");

		while(fPtr != NULL){

            setCleaner = fPtr;

            while (fPtr->firstJob != NULL) {
                cleaner = fPtr->firstJob;
                fPtr->firstJob = fPtr->firstJob->next;
                free(fPtr->firstJob);
            }

            fPtr = fPtr->nextSet;
            free(setCleaner);
		}

		free(save);
		free(queue);

	} while (quit != 'q');

	printf("Thanks for using the Chew Language Scheduling Algorithm Program.\n");
	sleep(5);

	return 0;
}
