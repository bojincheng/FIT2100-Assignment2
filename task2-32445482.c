/*
Author: Bojin Cheng
Start date: 12/09/2022
Last modified date: 01/10/2022

File header comments:
This file is the implementation of Round Robin scheduling algorithms
It contains a main function in which opens the file and scans the input file and then pass the scanned file into RB function in which is the implementation of RB scheduling algorithms
when finished the RB function call, it returns to the main function 
and write the new PCB data to a new created file
Please refer to the function header and inline comments for more detailed explanations of Round Robin function.
*/

#include "ass2.h"

/*
Function header comments:
Check if the given path is valid or not.

@param: char* path: path name of the file.
@return: boolean if whether the path is valid or not.
*/
bool is_valid_path(char * path) {
  return ((path[0] == '/') ? True : False);
}

/*
Function header comments:
this main function is used to open the file in which can be default "processes.txt"
file or promote the user to input the valid file path name.
Then, read the data inside the file, create PCB instances and append PCB into an array
Last, it will pass the PCB array into RR function.
Finally, the result file called "results-tasknum.txt" in the current directory.

@param: int argc: number of arguments that input in the.
@param: char* argv[]: an array of arguments that input in the terminal.
@return: 0 indicates the successful of executing the main function, else returns 1.
*/

//inline comments started with"//".

int main(int argc, char * argv[]) {
  FILE * file;
  //open the default file with read permission.
  file = fopen("processes.txt", "r");

  //if user input more than 1 argument that means the user has given a file name or path.
  //assuming the user input the argv[0] as object code name(./t2)
  //and followed by the argv[1] in which can be the absolute path or file name.
  if (argc > 1) {

    //if the input path res is an absolute path(starts with '/') by passing
    //the res variable to is_valid_path function.
    //use it instead of the default processes.txt file.
    if (is_valid_path(argv[1])) {

      //check if the given file is existed or not
      //if exists, open if for reading.
      if (access(argv[1], F_OK) == 0) {
        file = fopen(argv[1], "r");
      }

      //not existed, stand steam output to the terminal with error message
      else {
        write(2, strerror(errno), strlen(strerror(errno)));
        exit(1);
      }
    }

    //if given a valid filename instead of an absolute path
    else if (access(argv[1], F_OK) == 0) {
      file = fopen(argv[1], "r");
    }

    //if the argument res is not absolute path, output the error message.
    else {
      write(2, strerror(errno), strlen(strerror(errno)));
      exit(1);
    }
  }

  int data; //data being read from the file(processes.txt).
  char processName[10];
  int arrivalTime;
  int serviceTime;
  int deadline;
  int num_process = 0; //total number of processes
  pcb_t proc[100]; //allows the pcb to store up to 100 processes.

  //read each line from the file
  //by using the memory locations as scanf int variables
  data = fscanf(file, "%s %d %d %d", processName, & arrivalTime, & serviceTime, & deadline);

  //exit the loop if reach the end of the file.
  //store the data that read from the above and write into pcb_t array.
  while (data != EOF) {
    //pcb_t instance.
    pcb_t proc_data;
    //copy the processName in "processes.txt" file into proc_data array.
    strcpy(proc_data.process_name, processName);
    proc_data.entryTime = arrivalTime;
    proc_data.serviceTime = serviceTime;
    proc_data.remainingTime = serviceTime; //remained service time.
    proc_data.deadline = deadline;
    //append PCB into an array.
    //store proc_data into pcb_t proc[] as pass them as argument.
    //proc_data(scanned from above) has been assigned to each proc[] array with pcb_t struct.
    proc[num_process] = proc_data;
    num_process++;
    data = fscanf(file, "%s %d %d %d", processName, & arrivalTime, & serviceTime, & deadline);
  }
  //close the file.
  fclose(file);

  //passing the PCB array to FCFS function.
  RR(proc, num_process);

  //after calling the function, output the file called "results-2.txt" with write permission.
  FILE * outfile = fopen("results-2.txt", "w");
  if (outfile == NULL) {
    printf("Invalid file\n");
    return 1;
  }

  //loop the processes to write into outfile.
  for (int i = 0; i < num_process; i++) {
    //pcb_t instance
    pcb_t processes = proc[i];

    //check if success or not.
    int deadlineMet;
    if (processes.turnaroundTime <= processes.deadline) {

      //finished the execution.
      deadlineMet = 1;
    }
    //failed to finish the execution.
    else {
      deadlineMet = 0;
    }

    //write data into the new results-1.txt file.
    fprintf(outfile, "%s %d %d %d\n", processes.process_name, processes.waitTime, processes.turnaroundTime, deadlineMet);
  }
  fclose(outfile);
  return 0;
}

/*
Function header comments:
Function RB is used to calculate the time of RB algorithms. The time-slice quantum is 2 seconds
the new process will be prioritized
Process remaining time is used to see the remaining running time of the process, it will be decremented by one each time.
The turnaround time, wait time, and dealine met time will be calculated.

@param: pcb_t proc[]: pcb process array that read from the file.
@param: int _num_process: number of processes that in the pcb array.
@return: void function returns nothing.
*/

void RR(pcb_t proc[], int num_process) {  //pcb_t processes[] is the same as the proc[].
  //used as time counter.
  int duration = 0;
  boolean bools = False;
  int process_done = 0;
  pcb_t * running; //will be updated later depending on remaining time and running is used as an variable to access process state information.
  pcb_t * temp_proc[100]; //allows to store 100 processes as required.
  int temp_proc_ctr = 0;
  int head = 0;
  int quantum = 2; //time slice is 2 seconds.
  int quantum_ctr = 0;
  int executed_time = 0;

  while (process_done < num_process) {
    int i = 0;
    while (i < num_process) {
      //seek each data of processes from proc_data using pointer.
      pcb_t * processes = & proc[i];
      //pointer to access struct data type.
      //if entryTime == duration means the process just entered.
      //duration =0 so P1 enters.
      if (processes -> entryTime == duration) {
        //print message if the process has entered into the system and update its state as READY.
        fprintf(stdout, "Time %d: %s has entered the system.\n", duration, processes -> process_name);
        temp_proc[head] = processes;
        head++;
        temp_proc_ctr++;
        processes -> states = READY;
      }
      //incremented the loop.
      i++;
    }

    //process is running and remaining time is 0, exit.
    if (bools == True && running -> remainingTime == 0) {
      //turnaroundTime = completion - entryTime
      running -> turnaroundTime = duration - running -> entryTime;
      //waitingTime = turnaroundTimme - serviceTime
      running -> waitTime = running -> turnaroundTime - running -> serviceTime;
      running -> states = EXIT;
      fprintf(stdout, "Time %d: %s has finished the execution.\n", duration, running -> process_name);
      bools = False;
      process_done++;
      temp_proc_ctr--;
      quantum_ctr++;
      executed_time = 0;
    } 
    //put the unfinished process back to array when time slice = 2 is met.
    else if (bools == True && executed_time >= quantum && temp_proc_ctr > 0) {
      temp_proc_ctr++;
      running -> states = READY;
      executed_time = 0;
      temp_proc[head] = running;
      head++;
      quantum_ctr++;
      bools = False;
    }

    //if no process is running, put the next process into running state.
    //for P1 quantum_ctr is currently = 0 so temp_proc[0] is P1.
    if (bools == False && (running = temp_proc[quantum_ctr])) {
      temp_proc_ctr--;
      running -> states = RUNNING;
      fprintf(stdout, "Time %d: %s is in the running state.\n", duration, running -> process_name);
      bools = True;
    }
    if (running) {
      executed_time++;  //increments the executed_time.
      running -> remainingTime--;  //decrements the remainingTime.
    }
    duration++;
    sleep(1);
  }
}

