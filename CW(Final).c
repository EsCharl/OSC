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
	struct Job* next;
};

int main(void) {
	char fname[100];
	char inputMode;
	char exit;
	char blank;
	char selection;
	char quit;

	char Jobs[6][60] = { "Shortest Job First (SJF) Preemptive Version", "Shortest Job First (SJF) Non-Preemptive Version", "Round Robin (RR) with Overhead", "Round Robin (RR) without Overhead","Round Robin (RR) with Overhead without Arrival Time", "Round Robin (RR) without Overhead without Arrival Time" };

	int started = 0;
	int mode;

	double avgWaitTime[6], avgTurnaroundTime[6];

	typedef struct Job node;
	typedef node* nodePtr;

	nodePtr N;
	nodePtr sPtr;
	nodePtr cPtr;

	sPtr = malloc(sizeof(node));

	N = sPtr;
	cPtr = sPtr;

	FILE* read;

	printf("Welcome to the Scheduling Algorithm program!\n");

	printf("How would you like to input the inputs\n");
	while (!(inputMode == '1' || inputMode == '2')) {
		printf("Please enter a value of 1 or 2.\n");
		printf(" 1. Manually Input.\n 2. Input via .txt file.\n");

		scanf(" %c", &inputMode);
	}

	if (inputMode == '1') {
		while (exit != 'q') {
			printf("Job name?(no spaces)\n");
			scanf("%s", N->job_name);

			printf("Arrival Time? (Integer)\n");
			scanf("%d", &N->arrivalTime);

			printf("Burst Time (integer)\n");
			scanf("%d", &N->burst);

			N->completionTime = 0;
			N->turnaroundTime = 0;
			N->waitingTime = 0;
			N->burstLeft = 0;

			printf("Would you like to add more jobs? (q to quit)\n");

			scanf("%c", &blank);
			scanf("%c", &exit);

			if (exit != 'q') {
				N->next = malloc(sizeof(node));
				N = N->next;
			}
		}
	}
	else {
		printf("\nPlease input the file you'd like to load into the program.\n");
		printf("    *Please ensure the file is in .txt file\n    *Use ' , ' to seperate each category. the format of seperation should be: \n           (Job Name) , (Arrival time) , (Burst)\n\n    *Job Name Should not contain any spaces\n");
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
				N->burstLeft = 0;

				if (!feof(read)) {
					N->next = malloc(sizeof(node));
					N = N->next;
				} // end if
			} // end while

			fclose(read);
		} // end else
	}

	N->next = NULL;

	//return 0;

	while (!(selection == '1' || selection == '2' || selection == '3' || selection == '0' || selection == '4' || selection == '5')) {
		printf("\nPlease enter a value between 0 to 5.\n");
		printf("Please select one of the scheduling algorithm that you think will be the most suited given the workload given.\n 0. Shortest Job First (SJF) Preemptive Version. \n 1. Shortest Job First (SJF) Non-Preemptive Version. \n 2. Round Robin (RR) with Overhead. \n 3. Round Robin (RR) without Overhead.\n 4. Round Robin (RR) with Overhead without Arrival Time. \n 5. Round Robin (RR) without Overhead without Arrival Time.\n");

		scanf(" %c", &selection);
	}

	int sortFlag, systemClock = 0, hold, count = 0, breakFlag = 1, loop, lockFlag, size = 0, delayFlag = 0, delayTime, delay = 0, timeLeft, timeQuantum = 0;
	char nameHold[100];
	nodePtr temp, lPtr = NULL, compare;

	while (timeQuantum == 0) {
		printf("\nPlease input the time quantum.\n");
		scanf("%d", &timeQuantum); //get time quantum
	}

	while (delay == 0) { //make sure input is not 0
		printf("Please input the time delay.\n");
		scanf("%d", &delay); //get overhead
	}

	if (sPtr == NULL) { //if list is empty, end program
		printf("No processes found\n");
		return 0;
	}

	do { //sort processes according to arrival time
		sortFlag = 0; //sort flag
		temp = sPtr;

		while (temp->next != lPtr) {

			if (temp->arrivalTime > (temp->next)->arrivalTime) {

				strcpy(nameHold, temp->job_name);
				strcpy(temp->job_name, (temp->next)->job_name);
				strcpy((temp->next)->job_name, nameHold); //bubble sort job name
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
	} while (sortFlag);

	for (temp = sPtr; temp != NULL; temp = temp->next) { //get size of list
		size++;
	}

	nodePtr* queue; //array of pointers
	queue = malloc(sizeof(*queue) * (size + 2)); //get space for queue

	node* save; //array of structs
	save = malloc(sizeof(node) * 6 * size); //get space to save array results, 6 is for 6 algorithms

	for (mode = 0; mode < 6; mode++) { //loop through all modes

		for (temp = sPtr; temp != NULL; temp = temp->next) { //loop through list
			temp->burstLeft = temp->burst; //copy burst time to burst time left
			temp->waitingTimeSelected = 0; //reset first time processing flag
		}

		if (mode == 4 || mode == 5) { //for RR without arrival time

			for (temp = sPtr; temp != NULL; temp = temp->next) { //set all arrival time = 0
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

			for (temp = sPtr; temp != NULL; temp = temp->next) { //loop through list

				if (temp->arrivalTime == systemClock && temp->burstLeft != 0) { //if arrival time matches clock and burst time left

					if (queue[0] == NULL) { //if first node is empty
						queue[0] = temp; //fill first node
						lockFlag = 1; //to stop same loop operation
					}
					else { //if first node is filled

						for (loop = 0; loop < size; loop++) { //loop through queue

							if (queue[loop] == NULL) { //find empty node
								queue[loop] = temp; //fill empty node
								break;
							}
						}
					}
				}
			}

			if (delayFlag && (mode == 2 || mode == 4)) { //delay flag set

				delayTime--; //minus delay time

				if (delayTime == 0) { //if delay time finished
					delayFlag = 0; //turn off delay flag
				}

				continue; //go to next loop without processing
			}

			if (mode == 1) { //non-preemptive

				do {
					sortFlag = 0;

					for (loop = 1; loop < size; loop++) { //start from second node

						if (queue[loop] == NULL || queue[loop + 1] == NULL) { //if there are null pointers, break so no dereferencing occurs and causes error
							break; //break
						}

						if (queue[loop]->burstLeft > queue[loop + 1]->burstLeft) { //if current is larger than next number

							queue[size] = queue[loop];
							queue[loop] = queue[loop + 1];
							queue[loop + 1] = queue[size]; //bubble sort queue pointer
							queue[size] = NULL; //set temp pointer to NULL
							sortFlag = 1;
						}
					}
				} while (sortFlag);
			}

			if (queue[0] != NULL && !(lockFlag)) { //if process is in queue and not same loop

				if (mode == 2 || mode == 3 || mode == 4 || mode == 5) {
					timeLeft--; //minus time
				}

				queue[0]->burstLeft--; //minus burst
				if (queue[0]->waitingTimeSelected == 0) { //flag to check first time enter queue
					queue[0]->waitingTimeSelected = 1; //set flag
					queue[0]->waitingTime = (systemClock - 1) - queue[0]->arrivalTime; //systemClock - 1 because process is only processed on the next second
				}

				if (queue[0]->burstLeft == 0) { //if process completes

					queue[0]->completionTime = systemClock; //get completion time from clock
					queue[0]->turnaroundTime = (queue[0]->completionTime - queue[0]->arrivalTime); //get turnaround time from formula//get waiting time from formula

					for (loop = 0; loop < size; loop++) { //loop through queue
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

					for (loop = 0; loop < (size + 1); loop++) { //loop through queue

						if (queue[loop] == NULL) { //find empty node
							queue[loop] = queue[0]; //fill empty node
							break;
						}
					}

					for (loop = 0; loop < size; loop++) { //loop through queue
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

			if (lockFlag) { //prevent same loop running
				lockFlag = 0;
			}

			if (mode == 0) { //preemptive

				do {
					sortFlag = 0;

					for (loop = 0; loop < size; loop++) { //start from first node

						if (queue[loop] == NULL || queue[loop + 1] == NULL) { //if there are null pointers, break so no dereferencing occurs and causes error
							break; //break
						}

						if (queue[loop]->burstLeft > queue[loop + 1]->burstLeft) { //if current is larger than next number

							queue[size] = queue[loop];
							queue[loop] = queue[loop + 1];
							queue[loop + 1] = queue[size]; //bubble sort queue pointer
							queue[size] = NULL; //set temp pointer to NULL
							sortFlag = 1;
						}
					}
				} while (sortFlag);
			}

			breakFlag = 0; //break flag

			for (temp = sPtr; temp != NULL; temp = temp->next) { //if all processes ended
				if (temp->burstLeft != 0) {
					breakFlag = 1; //set flag
					break;
				}
			}
		}

		breakFlag = 1; //reset break flag;

		do{ //sort processes according to completion time
            sortFlag = 0; //sort flag
            temp = sPtr;

            while (temp->next != lPtr){

                if (temp->completionTime > (temp->next)->completionTime){

                    strcpy(nameHold,temp->job_name);
                    strcpy(temp->job_name,(temp->next)->job_name);
                    strcpy((temp->next)->job_name,nameHold); //bubble sort job name

                    hold = temp->arrivalTime;
                    temp->arrivalTime = (temp->next)->arrivalTime;
                    (temp->next)->arrivalTime = hold; //bubble sort arrival time

                    hold = temp->burst;
                    temp->burst = (temp->next)->burst;
                    (temp->next)->burst = hold; //bubble sort burst time

                    hold = temp->completionTime;
                    temp->completionTime = (temp->next)->completionTime;
                    (temp->next)->completionTime = hold; //bubble sort completion time

                    hold = temp->turnaroundTime;
                    temp->turnaroundTime = (temp->next)->turnaroundTime;
                    (temp->next)->turnaroundTime = hold; //bubble sort turnaround time

                    hold = temp->waitingTime;
                    temp->waitingTime = (temp->next)->waitingTime;
                    (temp->next)->waitingTime = hold; //bubble sort waiting time

                    sortFlag = 1;
                }
                temp = temp->next;
            }
            lPtr = temp;
        }while (sortFlag);

        lPtr = NULL; //reset pointer
		temp = sPtr; //reset pointer

		for (loop = size * mode; loop < size * (mode + 1); loop++) { //loop through save array

			strcpy(save[loop].job_name, temp->job_name);
			save[loop].burst = temp->burst;
			save[loop].completionTime = temp->completionTime;
			save[loop].turnaroundTime = temp->turnaroundTime;
			save[loop].waitingTime = temp->waitingTime;
			save[loop].arrivalTime = temp->arrivalTime;

			temp = temp->next;
		}

		for (temp = sPtr; temp != NULL; temp = temp->next) { //reset statistics

			temp->completionTime = 0;
			temp->turnaroundTime = 0;
			temp->waitingTime = 0;
		}

		printf("%s\n", Jobs[mode]);

		int sumWaitTime = 0, sumTurnaroundTime = 0;

		printf("%-15s | %-3s | %-3s : %-6s | %-6s | %-6s\n", "Job Name", "A.T", "B.T", "C.T", "T.A.T", "W.T");

		for (loop = size * mode; loop < size * (mode + 1); loop++) { //print out information from loop
			printf("%-15s | %-3d | %-3d : %-6d | %-6d | %-6d\n", save[loop].job_name, save[loop].arrivalTime, save[loop].burst, save[loop].completionTime, save[loop].turnaroundTime, save[loop].waitingTime);

			sumWaitTime += save[loop].waitingTime;
			sumTurnaroundTime += save[loop].turnaroundTime;
		}

		avgWaitTime[mode] = sumWaitTime / (double)size;
		avgTurnaroundTime[mode] = sumTurnaroundTime / (double)size;

		printf("Average Turnaround time: %0.3f | Average Waiting Time: %0.3f\n", avgTurnaroundTime[mode], avgWaitTime[mode]);

		printf("\n"); //get extra line

	}
	printf("A.T = Arrival Time.\nB.T = Burst Time\nC.T = Complete Time.\nT.A.T = Turnaround Time.\nW.T = Wait Time\n");

	printf("you've selected : %c. %s\n", selection, Jobs[selection - '0']);

	double lowestAvgWaitingTime = 999999999999999999, lowestAvgTurnaroundTime = 999999999999999999;
	int lowestAvgWaitingTimeIndex, lowestAvgTurnaroundTimeIndex, winner1 = 6, winner2 = 6, winner = 6;
	for (int i = 0; i < 6; i++) {
		if (lowestAvgWaitingTime >= avgWaitTime[i]) {
			lowestAvgWaitingTime = avgWaitTime[i];
			lowestAvgWaitingTimeIndex = i;
		}

		if (lowestAvgTurnaroundTime >= avgTurnaroundTime[i]) {
			lowestAvgTurnaroundTimeIndex = i;
			lowestAvgTurnaroundTime = avgTurnaroundTime[i];
		}
	}

	printf("\n");

	if (avgWaitTime[lowestAvgWaitingTimeIndex] == avgWaitTime[selection - '0'] || avgTurnaroundTime[lowestAvgTurnaroundTimeIndex] == avgTurnaroundTime[selection - '0']) {
		printf("You are RIGHT ");
		if (avgWaitTime[lowestAvgWaitingTimeIndex] == avgWaitTime[selection - '0'])
			winner = lowestAvgWaitingTimeIndex;
		else
			winner = lowestAvgTurnaroundTimeIndex;
	}
	else {
		printf("You are WRONG ");

		winner1 = lowestAvgTurnaroundTimeIndex;
		winner2 = lowestAvgWaitingTimeIndex;

		if (winner1 == winner2)
			winner = winner1;
	}

	if (winner < 6)
		printf("%s is the best algorithm based on this workload ", Jobs[winner]);

	if (avgWaitTime[lowestAvgWaitingTimeIndex] == avgWaitTime[selection - '0'] && avgTurnaroundTime[lowestAvgTurnaroundTimeIndex] == avgTurnaroundTime[selection - '0'])
		printf("in both lowest average wait time and lowest average turnaround time.\n");
	else if (avgWaitTime[lowestAvgWaitingTimeIndex] == avgWaitTime[selection - '0'])
		printf("in lowest average wait time.\n");
	else if (avgTurnaroundTime[lowestAvgTurnaroundTimeIndex] == avgTurnaroundTime[selection - '0'])
		printf("in lowest average turnaround time.\n");

	if (winner1 != winner2) {

		printf("%s is the best algorithm based on this workload for lowest average turnaround time and %s for the lowest average waiting time.\n", Jobs[winner1], Jobs[winner2]);

	}

	printf("\nLowest Avg Turnaround Time : %0.3f\n", avgTurnaroundTime[lowestAvgTurnaroundTimeIndex]);
	for (int i = 0; i < 6; i++) {
		if (avgTurnaroundTime[i] == avgTurnaroundTime[lowestAvgTurnaroundTimeIndex])
			printf("> %s\n", Jobs[i]);
	}

	printf("\nLowest Avg Waiting Time : %0.3f\n", avgWaitTime[lowestAvgWaitingTimeIndex]);
	for (int i = 0; i < 6; i++) {
		if (avgWaitTime[i] == avgWaitTime[lowestAvgWaitingTimeIndex])
			printf("> %s\n", Jobs[i]);
	}

	while (quit != 'q') {
		printf("\nto quit : 'q'\n");

		scanf(" %c", &quit);
	}

	printf("\nThanks for using the Chew Language Scheduling Algorithm Program.\n");
	sleep(5);

	return 0;

}