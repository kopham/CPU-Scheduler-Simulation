/* Khoi Pham
 * CSCI 3453
 * Due 04/04/23
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <queue>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

class process {
public:
    int pid;
    int arrival_time;
    int burst_time;
    int wait_time;
    int finish_time;
    int turn_around_time;
    int number_of_switches;
    int start_time;
    int burst_remaining;
};
//Read the file and make process from the information given
process makeProc(string line) {
    int procNum;
    int procRcvT;
    int procBurst;

    char delimiter = ' ';
    //Find the first space
    size_t pos = line.find(delimiter);
    int num = stoi(line.substr(0, pos));
    procNum = num;
    line.erase(0, pos);
    pos = line.find_first_not_of(delimiter);
    line.erase(0, pos);
    pos = line.find(delimiter);
    int num1 = stoi(line.substr(0, pos));
    procRcvT = num1;
    line.erase(0, pos);
    pos = line.find_first_not_of(delimiter);
    line.erase(0, pos);
    int num2 = stoi(line);
    procBurst = num2;
    process proc = {procNum, procRcvT, procBurst, 0, 0, 0, 0, 0, procBurst};
    return proc;
}

//Functions declaration
bool compareByArrival(const process& x, const process& y);
bool compareByPID(const process& x, const process& y);
bool compareByBurst(const process& x, const process& y);
bool compareByFinish(const process& x, const process& y);
void FCFS(vector<process>& p);
void ganttChartFCFS(vector<process> p, int q);
void SRTF(vector<process>& p);
void ganttChartSRTF(vector<process> p);
void RR(vector<process>& p, int q);
void ganttChartRR(vector<process> p, int q);
void displayTable(vector<process> p, int n);
void computeAverage(double total_burst_time, double total_wait_time, double total_turn_around_time, int n, int numContextSwitch);

//Main function
int main(int argc, char *argv[]) {

    ifstream input(argv[1]);
    string schedulingAlgo = argv[2];

    cout << schedulingAlgo << endl;
    vector<process> x;

    string line;
    //read the file and create proc struc and put it in a vector
    if (input.is_open()) {
        while (getline(input, line)) {
            x.push_back(makeProc(line));
        }
        input.close();
    }
    //check for all input cases
    if (schedulingAlgo == "FCFS") {
        FCFS(x);
        cout << endl;
        cout << "********************************************************************************************************\n";
        cout << "************************************ Scheduling algorithm : FCFS ***************************************\n";
        cout << "********************************************************************************************************\n";
        //Display the gantt chart for FCFS
        ganttChartFCFS(x, x.size());
        //Display the table for FCFS
        displayTable(x, x.size());
        return 0;
    }
    else if (schedulingAlgo == "SRTF") {
        SRTF(x);
        cout << endl;
        cout << "********************************************************************************************************\n";
        cout << "************************************ Scheduling algorithm : SRTF ***************************************\n";
        cout << "********************************************************************************************************\n";
        //Display the gantt chart for SRTF
        ganttChartSRTF(x);
        //Display the table for SRTF
        displayTable(x, x.size());
        return 0;
    }
    else if (schedulingAlgo == "RR") {
        RR(x, atoi(argv[3]));
        cout << endl;
        cout << "********************************************************************************************************\n";
        cout << "************************************ Scheduling algorithm : RR *****************************************\n";
        cout << "********************************************************************************************************\n";
        //Display the gantt chart for RR
        ganttChartRR(x,atoi(argv[3]));
        //Display the table for RR
        displayTable(x, x.size());
        return 0;
    }
    else {
        cout << "Error! Must enter correct scheduling algorithms (either FCFS or SRTF or RR).\n";
    }
    return 0;
}

bool compareByArrival(const process& x, const process& y) {
    return x.arrival_time < y.arrival_time;
}

bool compareByPID(const process& x, const process& y) {
    return x.pid < y.pid;
}

bool compareByBurst(const process& x, const process& y) {
    return x.burst_time < y.burst_time;
}

bool compareByFinish(const process& x, const process& y) {
    return x.finish_time > y.finish_time;
}

//FCFS scheduling algorithm
void FCFS(vector<process>& p) {
    int time = 0;
    int procNum = 0;
    int finish = 0;
    int currentTime = p[0].arrival_time;
    int numContextSwitch = 0;
    double total_wait_time = 0.0;
    double total_turn_around_time = 0.0;
    double total_burst_time = 0.0;

    sort(p.begin(), p.end(), compareByArrival);
    while (finish < p.size() || finish > p.size()) {
        if ((p[procNum].burst_time + currentTime) == time) {
            p[procNum].finish_time = time;
            p[procNum].turn_around_time = p[procNum].finish_time - p[procNum].arrival_time;
            p[procNum].wait_time = p[procNum].turn_around_time - p[procNum].burst_time;
            total_burst_time = total_burst_time + p[procNum].burst_time;
            total_wait_time = total_wait_time + p[procNum].wait_time;
            total_turn_around_time = total_turn_around_time + p[procNum].turn_around_time;
            currentTime = time;
            procNum = procNum + 1;
            finish = finish + 1;
            continue;
        }
        time++;
    }
    //print the stats after the loop
    computeAverage(total_burst_time, total_wait_time, total_turn_around_time, p.size(), numContextSwitch);
    return;
}

//Display gantt chart for FCFS
void ganttChartFCFS(vector<process> p, int q) {
    int i;
    int j;
    //Print top bar
    printf(" ");
    for (i = 0; i < q; i++) {
        for (j = 0; j < p[i].burst_time; j++)
            printf("**");
        printf(" ");
    }
    printf("\n|");

    //Print pid in the middle
    for (i = 0; i < q; i++) {
        for (j = 0; j < p[i].burst_time - 1; j++)
            printf(" ");
        printf("P%d", p[i].pid);
        for (j = 0; j < p[i].burst_time - 1; j++)
            printf(" ");
        printf("|");
    }
    printf("\n ");
    //Print bottom bar
    for (i = 0; i < q; i++) {
        for (j = 0; j < p[i].burst_time; j++)
            printf("**");
        printf(" ");
    }
    printf("\n");

    //Print the timeline
    printf("0");
    for (i = 0; i < q; i++) {
        for (j = 0; j < p[i].burst_time; j++)
            printf("  ");
        if (p[i].turn_around_time > 9)
            //Backspace
            printf("\b");
        printf("%d", p[i].finish_time);
    }
    printf("\n(each box represents one ms)\n");
}

//SRTF scheduling algorithm
void SRTF(vector<process>& p) {
    sort(p.begin(), p.end(), compareByArrival);
    int isDone[100];
    int numContextSwitch = 0;
    int currentTime = 0;
    int finish = 0;
    double total_wait_time = 0.0;
    double total_turn_around_time = 0.0;
    double total_burst_time = 0.0;
    cout << setprecision(2) << fixed;
    queue<int> ready_queue;
    ready_queue.push(0);
    memset(isDone, 0, sizeof(isDone));
    while (finish < p.size() || finish > p.size()) {
        int index = -1;
        int lowestRemain = 10000000;

        for (int procNum = 0; procNum < p.size(); procNum++) {
            //If the arrival time is smaller than current time and is done complete ,so 1 is complete and 0 is not
            if (p[procNum].arrival_time <= currentTime && isDone[procNum] == 0) {
                if (p[procNum].burst_remaining < lowestRemain) {
                    //the current br will update to be the lowest one and index = the process number
                    lowestRemain = p[procNum].burst_remaining;
                    index = procNum;
                }
                //If the lowestRM and the process BR == to each other
                if (p[procNum].burst_remaining == lowestRemain) {
                    //Use FCFS
                    if (p[procNum].arrival_time < p[index].arrival_time) {
                        lowestRemain = p[procNum].burst_remaining;
                        index = procNum;
                    }
                }
            }
        }
        if (index < -1 || index > -1) {
            //If the burst remain == burst time
            if (p[index].burst_remaining == p[index].burst_time) {
                p[index].start_time = currentTime;
            }
            p[index].burst_remaining = p[index].burst_remaining - 1;
            currentTime = currentTime + 1;
            //If the burst remain == 0, compute all the numbers needed.
            if (p[index].burst_remaining == 0) {
                p[index].finish_time = currentTime;
                p[index].turn_around_time = p[index].finish_time - p[index].arrival_time;
                p[index].wait_time = p[index].turn_around_time - p[index].burst_time;

                total_turn_around_time = total_turn_around_time + p[index].turn_around_time;
                total_wait_time = total_wait_time + p[index].wait_time;
                total_burst_time = total_burst_time + p[index].burst_time;

                isDone[index] = 1;
                finish = finish + 1;
            }
        }
        else {
            currentTime = currentTime + 1;
        }
    }
    computeAverage(total_burst_time, total_wait_time, total_turn_around_time, p.size(), numContextSwitch);
}

//Display gantt chart for SRTF
void ganttChartSRTF(vector<process> p) {
    sort(p.begin(), p.end(), compareByFinish);
    printf(" ");
    int index = 0;
    int maxTime = p[0].finish_time;
    int procNum = 0;
    int isDone[100];

    memset(isDone, 0, sizeof(isDone));

    for (int i = 0; i < maxTime; i++) {
        printf("**");
        printf(" ");
    }
    printf("\n|");
    //Sort by PID and print the first element
    sort(p.begin(), p.end(), compareByPID);
    printf("P%d", p[procNum].pid);
    printf("|");
    if (p[procNum].burst_time > 1) {
        p[procNum].burst_time = p[procNum].burst_time - 1;
        index = index + 1;
    }
    //Sort by burst time
    sort(p.begin(), p.end(), compareByBurst);
    while (index < maxTime) {
        //If the procNum is not the last element and bt of the current procnum and bt of the next procnum == each other,
        //use FCFS
        if (procNum < p.size() - 1 && (p[procNum].burst_time == p[procNum + 1].burst_time)) {
            if (p[procNum].arrival_time <= p[procNum + 1].arrival_time) {
                printf("P%d", p[procNum].pid);
                printf("|");
                p[procNum].burst_time = p[procNum].burst_time - 1;
                index = index + 1;
            }
            if (p[procNum].arrival_time > p[procNum + 1].arrival_time) {
                printf("P%d", p[procNum + 1].pid);
                printf("|");
                p[procNum + 1].burst_time = p[procNum + 1].burst_time - 1;
                sort(p.begin(), p.end(), compareByBurst);
                index = index + 1;
            }
        }
        //If the procNum is not the last element and
        //the burst time of current procNum is smaller than burst time of the next procNum
        if (procNum < p.size() - 1 && (p[procNum].burst_time < p[procNum + 1].burst_time)) {
            //print the current one
            printf("P%d", p[procNum].pid);
            printf("|");
            p[procNum].burst_time = p[procNum].burst_time - 1;
            index = index + 1;
        }
        if (p[procNum].burst_time == 0) {
            procNum++;
        }
        if (procNum == p.size() - 1) {
            printf("P%d", p[procNum].pid);
            printf("|");
            p[procNum].burst_time = p[procNum].burst_time - 1;
            index = index + 1;
        }
    }
    printf("\n");
    printf(" ");
    //Print bottom layer
    for (int i = 0; i < maxTime; i++) {
        printf("**");
        printf(" ");
    }
    printf("\n(each box represents one ms)\n");
}

//Round Robin scheduling algorithm
void RR(vector<process>& p, int q) {
    sort(p.begin(), p.end(), compareByArrival);
    int procNum;
    int numberContextSwitch = 0;
    int currentTime = 0;
    int finish = 0;
    int size[100];
    double total_wait_time = 0.0;
    double total_turn_around_time = 0.0;
    double total_burst_time = 0.0;

    cout << setprecision(2) << fixed;
    queue<int> readyqueue;
    readyqueue.push(0);
    memset(size, 0, sizeof(size));
    //Just to indicate that process in the queue, [0] indicates that process is not in the queue
    size[0] = 1;

    while (finish < p.size() || finish > p.size()) {
        procNum = readyqueue.front();
        readyqueue.pop();
        //If the burst_remaining is equal to the burst_time
        if (p[procNum].burst_remaining == p[procNum].burst_time) {
            //Then get the start_time of the process by take the larger between current time and arrival time
            p[procNum].start_time = max(currentTime, p[procNum].arrival_time);
            currentTime = p[procNum].start_time;
        }

        if (p[procNum].burst_remaining - q > 0) {
            p[procNum].burst_remaining = p[procNum].burst_remaining - q;
            currentTime = currentTime + q;
        }
        else {
            currentTime = currentTime + p[procNum].burst_remaining;
            //Set the process burst remaining equal to 0
            p[procNum].burst_remaining = 0;
            //Increment the finish for process
            finish = finish + 1;

            p[procNum].finish_time = currentTime;
            p[procNum].turn_around_time = p[procNum].finish_time - p[procNum].arrival_time;
            p[procNum].wait_time = p[procNum].turn_around_time - p[procNum].burst_time;

            total_burst_time = total_burst_time + p[procNum].burst_time;
            total_turn_around_time = total_turn_around_time + p[procNum].turn_around_time;
            total_wait_time = total_wait_time + p[procNum].wait_time;
        }
        //Check all of the processes that have burst remaining > 0 arrival time
        for (int i = 1; i < p.size(); i++) {
            if (p[procNum].burst_remaining > 0 && p[i].arrival_time <= currentTime &&
                size[i] == 0) {
                //Push the process in the ready queue
                readyqueue.push(i);
                size[i] = 1;
            }
        }
        if (p[procNum].burst_remaining > 0) {
            p[procNum].number_of_switches = p[procNum].number_of_switches + 1;
            numberContextSwitch = numberContextSwitch + 1;
            readyqueue.push(procNum);
        }
        //If the ready queue is empty, check is there are any process that burst remaining > 0 and have not been added to the queue.
        if (readyqueue.empty()) {
            for (int i = 1; i < p.size(); i++) {
                if (p[procNum].burst_remaining > 0) {
                    readyqueue.push(i);
                    size[i] = 1;
                    break;
                }
            }
        }
    }
    //Call the computeAverage function to do its tasks.
    computeAverage(total_burst_time, total_wait_time, total_turn_around_time, p.size(), numberContextSwitch);
}

//Display gantt chart for round robin
void ganttChartRR(vector<process> p, int q) {
    sort(p.begin(), p.end(), compareByFinish);
    printf(" ");
    int index = 0;
    int max_time = p[0].finish_time;
    int currentQ = 0;
    int procNum = 0;
    int isDone[100];
    int indexComp = 0;

    memset(isDone, 0, sizeof(isDone));
    sort(p.begin(), p.end(), compareByPID);
    //Display the top layer
    for (int i = 0; i < max_time; i++) {
        printf("**");
        printf(" ");
    }
    printf("\n|");
    //print the second layer
    while (index < max_time) {
        //If the currentQ is less than q and the process is not done printing and burst time is less than 0
        if ((currentQ < q) && (isDone[procNum] != 1) && (p[procNum].burst_time > 0)) {
            printf("P%d", p[procNum].pid);
            printf("|");
            currentQ = currentQ + 1;
            p[procNum].burst_time = p[procNum].burst_time - 1;
            index = index + 1;
        }
        // new
        if (currentQ == q) {
            //Reset the currentQ
            currentQ = 0;
            //If the current pid is equal to the last pid in the vector
            if (p[procNum].pid == p.back().pid) {
                procNum = 0;
            }
            //If the pid is not at the last element * new
            if (p[procNum].pid < p.back().pid || p[procNum].pid > p.back().pid) {
                //Increment it to the next procNum
                procNum = procNum + 1;
            }
        }
        if (p[procNum].burst_time == 0) {
            //Check to see if the procNum is located in the finish array
            bool remain = find(begin(isDone), end(isDone), p[procNum].pid) != end(isDone);
            if (!remain) {
                //Increment the indexComp
                indexComp = indexComp + 1;
                isDone[procNum] = p[procNum].pid;
                //If proNum is not at the back element
                if (p[procNum].pid < p.back().pid || p[procNum].pid > p.back().pid) {
                    procNum = procNum + 1;
                    currentQ = 0;
                    continue;
                }
                procNum = 0;
                currentQ = 0;
                continue;
            }
                // new. if they are exists and in the back, reset the procnum else continue to the next procnum
            else if (remain && p[procNum].pid == p.back().pid) {
                procNum = 0;
                continue;
            }
            procNum = procNum + 1;
        }
    }
    //print the last line
    printf("\n ");
    for (int i = 0; i < max_time; i++) {
        printf("**");
        printf(" ");
    }
    printf("\n(each box represents one ms)\n");
}

void displayTable(vector<process> p, int number) {
    int index;

    cout << "+-----+-------------+-------------+--------------+--------------+-----------------+--------------------+\n";
    cout << "| pid |   arrival   |  CPU-burst  |    finish    | waiting time |   turn around   |   No. of Context   |\n";
    cout << "+-----+-------------+-------------+--------------+--------------+-----------------+--------------------+\n";

    for (index = 0; index < number; index++) {
        printf("| %2d  |     %2d      |      %2d     |      %2d      |      %2d      |        %2d       |         %2d         |\n",
               p[index].pid, p[index].arrival_time, p[index].burst_time, p[index].finish_time,
               p[index].wait_time, p[index].turn_around_time, p[index].number_of_switches);
        cout <<"+-----+-------------+-------------+--------------+--------------+-----------------+--------------------+\n";
    }
}

//Compute and display the all total average time for all 3 scheduling algorithms
void computeAverage(double total_burst_time, double total_wait_time, double total_turn_around_time, int n, int numContextSwitch) {
    cout << fixed << setprecision(2);
    cout << "Average CPU burst time: " << (total_burst_time / n) << " ms" "\n";
    cout << "Average waiting time: " << (total_wait_time / n) << " ms" << "\n";
    cout << "Average turn around time: " << (total_turn_around_time / n) << " ms" << "\n";
    cout << "Total No. of Context Switching Performed = " << numContextSwitch << "\n";
}