How to run this code?
- double click on "Group(19)-OSC-CW-System.exe"

The program requires 2 sets of jobs at the minimum to function.
Each set of jobs require a minimum of 1 valid job each.

Each job contains a job name, arrival time and burst time.
For the job to be considered valid, there are 3 criteria:
1. Job name must not have spaces
2. Arrival time must be >= 0
3. Burst time must be >= 1

After entering the program, the program will ask the user to
enter their first set of jobs. There are 2 options for the 
user. 

Entering 1 allows user to manually input their jobs.
The program will ask for the job name, arrival time and
burst time of the first job. Any invalid inputs will result
in the program asking for a new, valid input. After the first
job is entered. The program will ask the user if he/she wishes
to enter more jobs. Press 'q' to quit and anything else to
enter a new job.

Entering 2 allows user to input the jobs via a text file.
The program will ask for the text file name. If the entered
name is invalid, the program will terminate. If the jobs are
invalid, the program will reject the invalid job and continue
reading the text file.

For the input text file:
- Format of jobs are (job name),(arrival time),(burst time) 
  without spaces
- Each new job is in a new line.
- No extra new lines or extra spaces.
- Example
  P1,1,6
  P2,2,5
  P3,0,12

After entering 2 sets of jobs, the program will ask the user if 
he/she wishes to enter more sets of jobs. Press 'q' to quit and 
anything else to enter a new set of jobs.

The program will then ask for the time quantum for the Round
Robin algorithms. Time quantum must be >0. Invalid inputs 
will result in the system asking for a new, valid input.

The program will then ask for the time delay for the algorithms
with delays. Time delay must be >0. Invalid inputs will result 
in the system asking for a new, valid input.

Then, the system will calculate the results based on 6 modes or
algorithms which are:
 1. Shortest Job First (SJF) Preemptive Version.
 2. Shortest Job First (SJF) Non-Preemptive Version.
 3. Round Robin (RR) with Overhead.
 4. Round Robin (RR) without Overhead.
 5. Round Robin (RR) with Overhead without Arrival Time.
 6. Round Robin (RR) without Overhead without Arrival Time.

The results for each algorithm will be presented in a table
for each set of jobs. There will also be a queue showing the
how the entire set of jobs is processed each second. The
results shown will include Entry Time, Completion Time,
Turnaround Time and Waiting Time. The results are sorted based 
on Completion Time.

Entry Time = Time job first enters front of queue
Completion Time = Time job completes processing
Turnaround Time = Completion Time - Arrival Time
Waiting Time = Entry Time - Arrival Time

Then, the lowest average turnaround time and waiting time
for each set of jobs will be listed with their respective
algorithm. If there are multiple algorithms with the same 
lowest times, they will also be listed. The average turnaround 
time and waiting time for each set of jobs will also be listed 
in ascending order.

Finally, the lowest average turnaround time and waiting time
for each algorithm will be listed with their respective
sets of job. If there are multiple sets of jobs with the same 
lowest times, they will also be listed.

After this, the user can press 'q' to quit the program or anything 
else to restart the program.

***************************************
Shown below are the main components contained within the program that
manages inbound jobs.

***************************************
System clock

Queue Insertion
	Lock Flag = true

Delay

Non-preemptive SJF Sorting

Main processing
	Check Lock Flag
	Burst Left = 0
	Time Quantum = 0

Lock Flag = false

Preemptive SJF Sorting

Break Flag check

******************************
System clock - The system clock is used to keep track of the time of
the program. When system clock shows second x, it means the interval
between second x-1 and x.

Queue Insertion - The queue insertion works by scanning the entire list
of jobs every second. As a result of this, jobs do not need to be 
sorted in anyway as the whole list is scanned every time. When arrival
time matches system clock, the job is inserted into the back of the
queue. If there are multiple jobs with the same arrival time, the job
in front of the list will be inserted first as the queue scans the 
list from front to back.

Lock Flag - The lock flag works by preventing same cycle processing.
Same cycle processing only becomes an issue when a job is inserted 
into an empty queue. In this instance, without a lock flag, the job 
will be processed in the same second as it is inserted. Imagine a job
entering at second 0 and already completing 1 cycle before a second
has elaspsed. Not logical is it? So, the lock flag activates when
insertion occurs in an empty queue. The lock flag is deactiavted after
passing through the main processing. The job is then processed in the
next second. Jobs inserted into a non-empty queue do not face this
issue so the lock flag is not triggered.

Delay - The delay works by continuing the queue before main processing
may occur when the delay flag is activated. It is used to simulate 
overhead. When overhead time passes, the delay flag is deactivated 
and normal processing may occur.

Non-preemptive SJF Sorting - Non-preemptive SJF Sorting works by
sorting the queue based on burst time left. The front of the queue is
left untouched by the sorting. So, any job which is currently
processing even if the burst time left is longer will be untouched
and allowed to process. Notice that the Non-preemptive SJF Sorting
is situated before the main processing. This ordering is important for
the program to work correctly. If the Non-preemptive SJF Sorting is
placed after main processing, a job may complete in the queue and
the next job is loaded before sorting can occur. As jobs at the front
are left untouched, this creates an issue.

Preemptive SJF Sorting - Preemptive SJF Sorting also works by sorting 
the queue based on burst time left. However, the front of the queue is
also subjected to the sorting. So, if any new job with a shorter burst
time left is detected, the job at the front of the queue will be 
replaced. Similar to Non-preemptive SJF Sorting, Preemptive SJF Sorting 
also has a specific ordering for the program to work correctly. 
Preemptive SJF Sorting must come after main processing. If Preemptive 
SJF Sorting is placed before main processing, a new job which enters
at the same second may replace the old job even before the old job
is processed.

Main processing - The main processing is the part which processes jobs
and decreases time quantum and burst time left. Before any job is
admitted, the lock flag is checked to prevent same cycle processing.
If the flag is false, then processing may occur. Burst time and time 
quantum are decreased every second. The main processing checks burst
left before time quantum because if burst time left is 0, then time
quantum is meaningless. If time quantum is reached, it means that there
is still burst time left. However, if time quantum is placed before
burst time, then if burst time and time quantum both reach 0, the job
is placed back into the queue even though burst time is 0. When burst
time is 0, completion time, turnaround time and waiting time are 
calculated. The queue is then moved forward, deleting the completed job
from the queue. When time quantum is 0, the job is moved to the back of
the queue. Then, the queue is moved forward.

Break Flag - The break flag is used to check if all jobs are completed
and to break out of the system clock. The break flag works by checking
all the jobs to see if the burst time left are all 0. If all jobs are
completed, then process breaks out of system clock. The advantage of
the break flag is that the loop ends in the same second as the last
job finishes. So, we do not need to specify a large number as the
loop termination condition.

***************************************
This program works by processing the job second by second. The program
works in a unique way in its time-keeping. When the system clock shows
second x, it actually means the interval between second x-1 to x. As an
example, second 2 means the interval between second 1 to 2. This may 
seem odd at first but it makes visualising the running of the code much
easier. As a result of this unique method of time-keeping, second 0 is
meaningless and is only used for job insertion.

To show the usefulness of this time-keeping method, we visualise 2 jobs
at second 3 using Round Robin algorithm. Job A finishes its time 
quantum at second 3 while Job B enters the queue at second 3. In normal
cases, this may produce a situation where the next job processed is
unclear. Is it Job A or Job B? With this time-keeping method, we can
safely say it is Job B every time. As mentioned above, second x 
actually means the interval between second x-1 to x. So, second 3 is 
actually the interval between second 2 and 3. So, when we say Job A
finishes its time quantum at second 3, we mean that Job A finishes its
job between second 2 and 3. Job B meanwhile is already inserted into
the queue as queue insertion comes before main processing. So, when 
Job A finishes its time quantum, it is moved to the back of the queue
while Job B which is already in the queue is moved to the front. So,
when we say Job B enters the queue at second 3, we mean that it enters
at the interval between second 2 and 3. Job B will only be processed
in the next second which is second 4 or the interval between second 3
and 4. As there is a clear order of processing for each second, we can
avoid situation which may produce ambiguity in the next job chosen.

***************************************
There are 6 different algorithms for job processing.
 1. Shortest Job First (SJF) Preemptive Version.
 2. Shortest Job First (SJF) Non-Preemptive Version.
 3. Round Robin (RR) with Overhead.
 4. Round Robin (RR) without Overhead.
 5. Round Robin (RR) with Overhead without Arrival Time.
 6. Round Robin (RR) without Overhead without Arrival Time.

Overhead is the time to switch between different processes for the
Round Robin algorithm. So, processes without overhead assumes that
the next job is loaded instantly when the old job ends. For jobs 
without arrival time, we mean to say that all jobs arrive at second 0.
So, all jobs are entered into the queue instantly in the order that
they are entered.

****************************************
The processes involved in each algorithm is listed below.

 1. Shortest Job First (SJF) Preemptive Version.

System clock

Queue Insertion
	Lock Flag = true

Main processing
	Check Lock Flag
	Burst Left = 0

Lock Flag = false

Preemptive SJF Sorting

Break Flag check

**************************************
 2. Shortest Job First (SJF) Non-Preemptive Version.

System clock

Queue Insertion
	Lock Flag = true

Non-preemptive SJF Sorting

Main processing
	Check Lock Flag
	Burst Left = 0

Lock Flag = false

Break Flag check

***************************
 3. Round Robin (RR) with Overhead.

System clock

Queue Insertion
	Lock Flag = true

Delay

Main processing
	Check Lock Flag
	Burst Left = 0
	Time Quantum = 0

Lock Flag = false

Break Flag check

****************************
 4. Round Robin (RR) without Overhead.

System clock

Queue Insertion
	Lock Flag = true

Main processing
	Check Lock Flag
	Burst Left = 0
	Time Quantum = 0

Lock Flag = false

Break Flag check

******************************
 5. Round Robin (RR) with Overhead without Arrival Time.

System clock

Queue Insertion (Once)
	Lock Flag = true

Delay

Main processing
	Check Lock Flag
	Burst Left = 0
	Time Quantum = 0

Lock Flag = false

Break Flag check

********************************
 6. Round Robin (RR) without Overhead without Arrival Time.

System clock

Queue Insertion (Once)
	Lock Flag = true

Main processing
	Check Lock Flag
	Burst Left = 0
	Time Quantum = 0

Lock Flag = false

Break Flag check
