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

    while (!(mode == '1' || mode == '2' || mode == '3')) {
        printf("Please enter a value between 1 to 3.\n");
        printf("Please select one of the scheduling algorithm that you wish to use.\n 1. Shortest Job First (SJF) Non-Preemptive Version. \n 2. Round Robin (RR) with Overhead. \n 3. Round Robin (RR) without Overhead.\n");

        scanf(" %c", &mode);
    }

    int sortFlag, flag, doneFlag, overheadFlag, newHead = 0, systemClock = 0, timeQuantum, delay = 0, hold, queue;
    char nameHold[100];
    nodePtr temp, count, lPtr = NULL;

    printf("Please input the time quantum.\n");
    scanf("%d", &timeQuantum); //get time quantum

    if (mode == '2') {
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

    for (temp = sPtr; temp != NULL; temp = temp->next) { //copy burst time to burst tie left
        temp->burstLeft = temp->burst;
    }

    printf("\n");

    do {
        doneFlag = 0; //done flag
        flag = 0;

        for (temp = sPtr; temp != NULL; temp = temp->next) {

            if (temp->burstLeft != 0)
                doneFlag++;

            if (temp->burstLeft > timeQuantum) {
                temp->burstLeft -= timeQuantum;
                systemClock += timeQuantum;
                systemClock += delay;
                flag++;
            }
            else if (temp->burstLeft <= timeQuantum && temp->burstLeft != 0) {
                systemClock += temp->burstLeft;
                temp->burstLeft -= temp->burstLeft;
                temp->completionTime = systemClock;
                temp->turnaroundTime = temp->completionTime;
                temp->waitingTime = temp->turnaroundTime - temp->burst;              
                systemClock += delay;
                flag++;
            }
        }
    } while (doneFlag); //flag to continue loop

    for (temp = sPtr; temp != NULL; temp = temp->next) { //print out all information
        printf("%s , %d , %d : %d %d %d %d\n", temp->job_name, temp->arrivalTime, temp->burst, temp->burstLeft, temp->completionTime, temp->turnaroundTime, temp->waitingTime);
    }

    return 0;

}