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
    struct Job* next;
};

int main(void) {
    char fname[100];
    char mode;
    char inputMode;
    char exit;
    char blank;

    int started = 0;

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
        printf(" 1. Manually Input.\n 2. Input via file. (please ensure the file is in .txt file and only consist of ' , ' to seperate each category. the order of seperation is \"Job name(no spaces)\" , \"Arrival time (please use '.' instead of ':' in seconds)\" , \"Burst (Time needed for the job)(please use '.' instead of ':' in seconds)\")\n");

        scanf(" %c", &inputMode);
    }

    if (inputMode == '1') {
        while (exit != 'q') {
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

            if (exit != 'q') {
                N->next = malloc(sizeof(node));
                N = N->next;
            }
        }
    }
    else {
        printf("Please input the file you'd like to load into the program.\n");
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

    while (!(mode == '1' || mode == '2' || mode == '3' || mode == '0' || mode == '4' || mode == '5')) {
        printf("Please enter a value between 0 to 5.\n");
        printf("Please select one of the scheduling algorithm that you wish to use.\n 0. Shortest Job First (SJF) Preemptive. \n 1. Shortest Job First (SJF) Non-Preemptive Version. \n 2. Round Robin (RR) with Overhead. \n 3. Round Robin (RR) without Overhead.\n 4. Round Robin+ with Overhead. \n 5.Round Robin+ without Overhead. \n");

        scanf(" %c", &mode);
    }

    int sortFlag, systemClock = 0, hold, count = 0, breakFlag = 1, loop, lockFlag, size = 0;
    char nameHold[100];
    nodePtr temp, lPtr = NULL;

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

    for (temp = sPtr; temp != NULL; temp = temp->next) { //copy burst time to burst time left
        temp->burstLeft = temp->burst;
        size++; //get size of list
    }

    nodePtr* queue; //array of pointers
    queue = malloc(sizeof(*queue) * (size + 2)); //get space

    node* save;
    save = malloc(sizeof(node) * size);

    for (loop = 0; loop < (size + 2); loop++) { //get one more extra space
        queue[loop] = NULL; //set null
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

        if (mode == '1') { //non-preemptive

            do {
                sortFlag = 0;

                for (loop = 1; loop < size; loop++) { //for preemptive loop = 0, non-preemptive loop = 1

                    if (queue[loop] == NULL || queue[loop + 1] == NULL) { //if there are null pointers, break so no dereferencing occurs and causes error
                        break; //break
                    }

                    if (queue[loop]->burstLeft > queue[loop + 1]->burstLeft || (queue[loop]->burstLeft == queue[loop + 1]->burstLeft && queue[loop]->burst > queue[loop + 1]->burst)) { //if current is larger than next number

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

            queue[0]->burstLeft--; //minus burst

            if (queue[0]->burstLeft == 0) { //if process completes

                queue[0]->completionTime = systemClock; //get completion time from clock
                queue[0]->turnaroundTime = (queue[0]->completionTime - queue[0]->arrivalTime); //get turnaround time from formula
                queue[0]->waitingTime = (queue[0]->turnaroundTime - queue[0]->burst); //get waiting time from formula

                for (loop = 0; loop < size; loop++) { //loop through queue
                    queue[loop] = queue[loop + 1]; //move jobs to the front
                    queue[loop + 1] = NULL; //set last queue as NULL
                }
            }
        }

        if (mode == '0') { //preemptive

            do {
                sortFlag = 0;

                for (loop = 0; loop < size; loop++) { //for preemptive loop = 0, non-preemptive loop = 1

                    if (queue[loop] == NULL || queue[loop + 1] == NULL) { //if there are null pointers, break so no dereferencing occurs and causes error
                        break; //break
                    }

                    if ((queue[loop]->burstLeft == queue[loop + 1]->burstLeft && queue[loop]->arrivalTime > queue[loop + 1]->arrivalTime) || queue[loop]->burstLeft > queue[loop + 1]->burstLeft) {
                        queue[size] = queue[loop];
                        queue[loop] = queue[loop + 1];
                        queue[loop + 1] = queue[size]; //bubble sort queue pointer
                        queue[size] = NULL; //set temp pointer to NULL
                        sortFlag = 1;
                    }
                }
            } while (sortFlag);
        }

        if (lockFlag) { //prevent same loop running
            lockFlag = 0;
        }

        breakFlag = 0; //break flag

        for (temp = sPtr; temp != NULL; temp = temp->next) { //if all processes ended
            if (temp->burstLeft != 0) {
                breakFlag = 1; //set flag
                break;
            }
        }
    }

    temp = sPtr;

    for (loop = 0; loop < size; loop++) {
        strcpy(save[loop].job_name, temp->job_name);
        save[loop].arrivalTime = temp->arrivalTime;
        save[loop].burst = temp->burst;
        save[loop].burstLeft = temp->burstLeft;
        save[loop].completionTime = temp->completionTime;
        save[loop].turnaroundTime = temp->turnaroundTime;
        save[loop].waitingTime = temp->waitingTime;

        temp = temp->next;
    }

    for (int i = 0; i < size; i++) { //print out all information
        printf("%s , %d , %d : %d %d %d %d\n", save[i].job_name, save[i].arrivalTime, save[i].burst, save[i].burstLeft, save[i].completionTime, save[i].turnaroundTime, save[i].waitingTime);
    }

    return 0;

}