/*StudentID:32445482
Date created: 17/09/2022
Last modified: 09/10/2022
Header file for task1-32445482.c, task2-32445482.c, task3-32445482.c
*/

/*File header comments:
typedef emum and typedef struct were taken from the assignment brief.
This header file includes the functions prototypes, library and defined variable
enumerated data type, and struct data type that are needed to compile
with task1-32445482.c, task2-32445482.c and task3-32445482.c
*/

#include <stdio.h>   //file manipulation.

#include <unistd.h>   //use for access file.

#include <stdlib.h>   //use for exit file.

#include <string.h>   //use for string operation.

#include <errno.h>  //output error number.

#include <stdbool.h>  //use for the boolean.

#define MAX_deadline 1000 //assume max number of seconds of the deadline to be 1000 seconds.

/* Special enumerated data type for process state */
typedef enum {
  READY,
  RUNNING,
  EXIT
}
process_state_t;

/*enumerated data type for process*/
typedef enum {
  True,
  False
}
boolean;

/* C data structure used as process control block(PCB). The scheduler
 * should create one instance per running process in the system.
 */

typedef struct {
  char process_name[11]; // A string that identifies the process

  /* Times are measured in seconds. */
  int entryTime; //The time process entered the system
  int serviceTime; //The total CPU time required by the process
  int remainingTime; //Remaining service time until the completion
  int deadline; //max time for the process to complete(completion time - arrival time)
  int turnaroundTime;
  int waitTime;
  process_state_t states; //Current process state, the instance of process_state_t defined above will be updated duration the processes lifetime.
}
pcb_t;

/*Function prototypes*/
bool is_valid_path(char * path);
void FCFS(pcb_t process[], int num_process);
void RR(pcb_t process[], int num_process);
void DLB(pcb_t process[], int num_process);
