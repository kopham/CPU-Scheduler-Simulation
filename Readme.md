###### Name: Khoi Pham
###### ID: 103270643
###### CSCI 3453
###### Lab 2
###### Due: Apr/04/2023

**********************************
## Description Of Program

#### The implementation details

Write a program to simulate a CPU scheduler which selects a process from a ready queue and executes the process by using the given scheduling algorithm, display its actives and evaluate its performance based on measurements such as average turn-around time, average waiting time, and total number of context switching. When a process is scheduled, the simulator will simply print what process is running at what time, collecting data and producing Gantt Chart-like outputs.

#### Specifications

You have to implement FCFS (First Come First Serve), SRTF (Shortest Remaining Task First – preemptive), and RR (Round Robin) scheduling algorithms.

#### Assumptions

*   Use the following assumptions when you design and implement your CPU simulator.
    *   There is only one CPU
    *   All processes perform only CPU operations.
    *   All processes have the same priority.
    *   The newly arrived process is directly added to the back of the ready queue.
    *   We use only ready queue for this simulation.
    *   Context switching is done in 0 ms.
    *   Context switch is performed only when a current process is moved to the back of ready queue.
    *   All time are given milliseconds.
    *   Use FCFS policy for breaking the tie.
    
####    Measurements and Evaluation

*   You program should collect the following information about each process:
    *   Time of completion
    *   Waiting time
    *   Turn around time
    *   No. of Context Switching

*   You program should calculate the following information using the collected data:
    *   Average CPU burst time
    *   Average waiting time
    *   Average turn around time
    *   Average response time
    *   Total number of Context Switching performed
    
#### Simulator Input

*   Process information (assume a maximum of 100 processes) will be read from a text file. The information for each process will include the following fields:
    *   pid: a unique numeric process ID.
    *   arrival time: arrival time for a process in ms.
    *   CPU burst time: the CPU time requested by a processes

*   You can assume that all time values are integers and pids provided in the input file are unique.

*   Name of the input file, scheduling algorithm and quantumsize (for RR algorithm) will be provided as command line argument as follows:

    *   myscheduler input_file FCFS
    *   myscheduler input_file SRTF
    *   myschedular input_file RR 4


********************************************
## Source Files

*   main.cpp

********************************************
## Status Of Program

*   The program of lab 2 was developed and tested on C++. It was successfully compiled and ran on Linux server (CSEGRID).

***********************************************
## How to build and run the program

1. Uncompress the homework. The homework file is compressed.  
    *   To uncompress it use the following commands: % unzip Khoi_Pham_Lab2
    *   Now you should see a directory named homework with the files:
        *  main.cpp
        *  lab2.txt
        *  makefile
        *  Readme.md
        *  FCFS Output
        *  SRTF Output
        *  RR Output
     
2. Build the program.
   *    Change to the directory that contains the file by: % cd Khoi_Pham_Lab2
   *    Compile the program by: % make
   
3. Run the program by: % 
    *   ./myscheduler lab2.txt FCFS  
    *   ./myscheduler lab2.txt SRTF  
    *   ./myscheduler lab2.txt RR 4

4. Delete the obj files, executables, and core dump by: % make clean
