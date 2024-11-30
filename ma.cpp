//Write a program (using fork () and/or exec () commands) where parent and child execute:
i. same program, same code.
ii. same program, different code.
iii. before terminating, the parent waits for the child to finish its task.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork(); // Create child process

    if (pid < 0) {
        // Error occurred
        printf("Fork failed\n");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        printf("Child Process (PID: %d)\n", getpid());

        // Case i: Same program, same code
        printf("Case i: Child executing same program, same code\n");

        // Case ii: Simulate different code in the same program
        printf("Case ii: Child executing a different block of code\n");
        printf("Child is performing its own unique task...\n");

        // Terminate child process
        printf("Child Process (PID: %d) is terminating.\n", getpid());
        exit(0);
    }
    else {
        // Parent process
        printf("Parent Process (PID: %d)\n", getpid());

        // Case iii: Parent waits for child to finish
        printf("Case iii: Parent waiting for child to complete\n");
        wait(NULL); // Wait for child to finish
        printf("Parent Process: Child completed\n");
    }

    return 0;
}

----------------------------------------------------------------------------------------------------------------------------------

//Write a program to report behaviour of Linux kernel including kernel version, CPU type and CPU
information.

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void printKernelVersion() {
    ifstream file("/proc/version");
    if (!file.is_open()) {
        cerr << "Error opening /proc/version" << endl;
        return;
    }

    string line;
    if (getline(file, line)) {
        cout << "Kernel Version: " << line << endl;
    }
    file.close();
}

void printCPUInfo() {
    ifstream file("/proc/cpuinfo");
    if (!file.is_open()) {
        cerr << "Error opening /proc/cpuinfo" << endl;
        return;
    }

    string line;
    cout << "CPU Information:" << endl;
    
    while (getline(file, line)) {
        // Print lines containing specific CPU information
        if (line.find("model name") != string::npos ||
            line.find("vendor_id") != string::npos ||
            line.find("cpu cores") != string::npos ||
            line.find("siblings") != string::npos ||
            line.find("cpu MHz") != string::npos) {
            cout << line << endl;
        }
    }
    file.close();
}

int main() {
    printKernelVersion();
    printCPUInfo();
    return 0;
}

-------------------------------------------------------------------------------------------------------------------------------------

//Write a program to report behaviour of Linux kernel including information on configured memory, amount of
free and used memory. (Memory information)

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void readFileAndPrintMemoryInfo(const string& filePath) {
    ifstream file(filePath);
    if (file.is_open()) {
        string line;
        string memTotal, memFree, memAvailable, buffers, cached;

        while (getline(file, line)) {
            if (line.find("MemTotal:") == 0) {
                memTotal = line;
            } else if (line.find("MemFree:") == 0) {
                memFree = line;
            } else if (line.find("MemAvailable:") == 0) {
                memAvailable = line;
            } else if (line.find("Buffers:") == 0) {
                buffers = line;
            } else if (line.find("Cached:") == 0) {
                cached = line;
            }
        }

        // Print the gathered memory information
        cout << "Memory Information: " << endl;
        cout << memTotal << endl;
        cout << memFree << endl;
        cout << memAvailable << endl;
        cout << buffers << endl;
        cout << cached << endl;

        file.close();
    } else {
        cerr << "Failed to open " << filePath << endl;
    }
}

int main() {
    // Reading and printing memory information from /proc/meminfo
    readFileAndPrintMemoryInfo("/proc/meminfo");

    return 0;
}

----------------------------------------------------------------------------------------------------------------------------

//Write a program to copy files using system calls.

#include <iostream>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main() {
    // Open a file for reading only
    int readFile = open("file.txt", O_RDONLY);
    if (readFile == -1) {
        cerr << "Failed to open file for reading" << endl;
    } else {
        cout << "File opened for reading successfully" << endl;
        close(readFile);
    }

    // Open a file for writing only
    int writeFile = open("file.txt", O_WRONLY);
    if (writeFile == -1) {
        cerr << "Failed to open file for writing" << endl;
    } else {
        cout << "File opened for writing successfully" << endl;
        close(writeFile);
    }

    return 0;
}

--------------------------------------------------------------------------------------------------------------------------------

//Write a program to implement FCFS scheduling algorithm.

#include <iostream>
using namespace std;

struct Process {
    int id;           // Process ID
    int arrivalTime;  // Arrival time
    int burstTime;    // Burst time (CPU time required)
    int startTime;    // Start time of the process
    int completionTime; // Completion time
    int waitingTime;  // Waiting time
    int turnaroundTime; // Turnaround time
};

void FCFS(Process processes[], int n) {
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;

    // Sorting processes by arrival time using a simple sorting algorithm (Bubble Sort)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[i].arrivalTime > processes[j].arrivalTime) {
                // Swap the processes
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    // Set the start time and completion time for the first process
    processes[0].startTime = processes[0].arrivalTime;
    processes[0].completionTime = processes[0].startTime + processes[0].burstTime;
    processes[0].waitingTime = processes[0].startTime - processes[0].arrivalTime;
    processes[0].turnaroundTime = processes[0].completionTime - processes[0].arrivalTime;

    // For each subsequent process, calculate the start, completion, waiting and turnaround time
    for (int i = 1; i < n; i++) {
        // The start time is the completion time of the previous process
        processes[i].startTime = processes[i-1].completionTime;

        // Completion time is start time + burst time
        processes[i].completionTime = processes[i].startTime + processes[i].burstTime;

        // Waiting time is the start time - arrival time
        processes[i].waitingTime = processes[i].startTime - processes[i].arrivalTime;

        // Turnaround time is completion time - arrival time
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
    }

    // Calculate the total waiting and turnaround time
    for (int i = 0; i < n; i++) {
        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    // Output the process details
    cout << "Process ID | Arrival Time | Burst Time | Start Time | Completion Time | Waiting Time | Turnaround Time" << endl;
    for (int i = 0; i < n; i++) {
        cout << processes[i].id << "          | " 
             << processes[i].arrivalTime << "          | "
             << processes[i].burstTime << "        | "
             << processes[i].startTime << "        | "
             << processes[i].completionTime << "          | "
             << processes[i].waitingTime << "           | "
             << processes[i].turnaroundTime << endl;
    }

    // Calculate and display the average waiting and turnaround time
    cout << "\nAverage Waiting Time: " << (float)totalWaitingTime / n << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / n << endl;
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    Process processes[n];  // Declare array of processes

    // Take input from the user for each process
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;  // Process ID starts from 1
        cout << "Enter Arrival Time and Burst Time for Process " << i + 1 << ": ";
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
    }

    // Call FCFS scheduling function
    FCFS(processes, n);

    return 0;
}

------------------------------------------------------------------------------------------------------------------------------------

//Write a program to implement SJF scheduling algorithm (non-preemptive)

#include <iostream>
#include <algorithm>
using namespace std;

struct Process {
    int id;            // Process ID
    int arrivalTime;   // Arrival time
    int burstTime;     // Burst time (CPU time required)
    int completionTime;// Completion time
    int waitingTime;   // Waiting time
    int turnaroundTime;// Turnaround time
};

bool compare(Process p1, Process p2) {
    return p1.burstTime < p2.burstTime; // Compare based on burst time
}

void SJF(Process processes[], int n) {
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;

    // Sort processes based on burst time (shortest burst time first)
    sort(processes, processes + n, compare);

    // Set the start time and completion time for the first process
    processes[0].completionTime = processes[0].arrivalTime + processes[0].burstTime;
    processes[0].waitingTime = 0; // No waiting for the first process
    processes[0].turnaroundTime = processes[0].completionTime - processes[0].arrivalTime;

    // For each subsequent process, calculate the completion, waiting, and turnaround time
    for (int i = 1; i < n; i++) {
        processes[i].completionTime = processes[i-1].completionTime + processes[i].burstTime;
        processes[i].waitingTime = processes[i].completionTime - processes[i].arrivalTime - processes[i].burstTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
    }

    // Calculate the total waiting and turnaround time
    for (int i = 0; i < n; i++) {
        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    // Output the process details
    cout << "Process ID | Arrival Time | Burst Time | Completion Time | Waiting Time | Turnaround Time" << endl;
    for (int i = 0; i < n; i++) {
        cout << processes[i].id << "          | "
             << processes[i].arrivalTime << "          | "
             << processes[i].burstTime << "        | "
             << processes[i].completionTime << "         | "
             << processes[i].waitingTime << "          | "
             << processes[i].turnaroundTime << endl;
    }

    // Calculate and display the average waiting and turnaround time
    cout << "\nAverage Waiting Time: " << (float)totalWaitingTime / n << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / n << endl;
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    Process processes[n];  // Declare array of processes

    // Take input from the user for each process
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;  // Process ID starts from 1
        cout << "Enter Arrival Time and Burst Time for Process " << i + 1 << ": ";
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
    }

    // Call SJF scheduling function
    SJF(processes, n);

    return 0;
}

-----------------------------------------------------------------------------------------------------------------------------------

//Write a program to implement non-preemptive priority-based scheduling algorithm.

#include <iostream>
#include <algorithm>
using namespace std;

struct Process {
    int id;             // Process ID
    int arrivalTime;    // Arrival time
    int burstTime;      // Burst time (CPU time required)
    int priority;       // Priority of the process
    int completionTime; // Completion time
    int waitingTime;    // Waiting time
    int turnaroundTime; // Turnaround time
};

// Function to sort processes by their priority
bool compareByPriority(Process p1, Process p2) {
    return p1.priority < p2.priority;
}

void priorityScheduling(Process processes[], int n) {
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int currentTime = 0;
    int completed = 0;

    // Sort processes by priority
    sort(processes, processes + n, compareByPriority);

    // Initializing processes to make sure that no process is completed initially
    while (completed < n) {
        bool processFound = false;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].burstTime > 0) {
                // Process found with the highest priority
                processes[i].completionTime = currentTime + processes[i].burstTime;
                processes[i].waitingTime = currentTime - processes[i].arrivalTime;
                processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;

                // Add the current process's waiting and turnaround time to totals
                totalWaitingTime += processes[i].waitingTime;
                totalTurnaroundTime += processes[i].turnaroundTime;

                // Mark the process as completed (set burst time to 0)
                processes[i].burstTime = 0;
                completed++;

                // Move the current time forward to the completion time of the current process
                currentTime = processes[i].completionTime;
                processFound = true;
                break;
            }
        }

        // If no process can be executed (all processes arrived later)
        if (!processFound) {
            currentTime++;
        }
    }

    // Output the process details
    cout << "Process ID | Arrival Time | Burst Time | Priority | Completion Time | Waiting Time | Turnaround Time" << endl;
    for (int i = 0; i < n; i++) {
        cout << processes[i].id << "          | "
             << processes[i].arrivalTime << "          | "
             << processes[i].burstTime << "        | "
             << processes[i].priority << "        | "
             << processes[i].completionTime << "         | "
             << processes[i].waitingTime << "          | "
             << processes[i].turnaroundTime << endl;
    }

    // Calculate and display the average waiting and turnaround time
    cout << "\nAverage Waiting Time: " << (float)totalWaitingTime / n << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / n << endl;
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    Process processes[n];  // Declare array of processes

    // Take input from the user for each process
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;  // Process ID starts from 1
        cout << "Enter Arrival Time, Burst Time, and Priority for Process " << i + 1 << ": ";
        cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
    }

    // Call priority scheduling function
    priorityScheduling(processes, n);

    return 0;
}

--------------------------------------------------------------------------------------------------------------------------------

//Write a program to implement SRTF scheduling algorithm.

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int id;             // Process ID
    int arrivalTime;    // Arrival time
    int burstTime;      // Total burst time (CPU time required)
    int remainingTime;  // Remaining burst time
    int completionTime; // Completion time
    int waitingTime;    // Waiting time
    int turnaroundTime; // Turnaround time
};

// Comparator to sort by arrival time
bool compareByArrival(Process p1, Process p2) {
    return p1.arrivalTime < p2.arrivalTime;
}

void srtfScheduling(Process processes[], int n) {
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int currentTime = 0;  // Time starts from 0
    int completed = 0;    // Number of completed processes
    int prevProcess = -1; // To store previously executed process

    // Sort the processes based on arrival time
    sort(processes, processes + n, compareByArrival);

    while (completed < n) {
        // Find the process with the shortest remaining time
        int minRemainingTime = INT_MAX;
        int shortestProcess = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 && processes[i].remainingTime < minRemainingTime) {
                minRemainingTime = processes[i].remainingTime;
                shortestProcess = i;
            }
        }

        if (shortestProcess != -1) {
            // Execute the selected process
            processes[shortestProcess].remainingTime--;

            // If process is completed
            if (processes[shortestProcess].remainingTime == 0) {
                processes[shortestProcess].completionTime = currentTime + 1;
                processes[shortestProcess].turnaroundTime = processes[shortestProcess].completionTime - processes[shortestProcess].arrivalTime;
                processes[shortestProcess].waitingTime = processes[shortestProcess].turnaroundTime - processes[shortestProcess].burstTime;

                totalWaitingTime += processes[shortestProcess].waitingTime;
                totalTurnaroundTime += processes[shortestProcess].turnaroundTime;
                completed++;
            }
        }

        // Increment time
        currentTime++;
    }

    // Output the process details
    cout << "Process ID | Arrival Time | Burst Time | Completion Time | Waiting Time | Turnaround Time" << endl;
    for (int i = 0; i < n; i++) {
        cout << processes[i].id << "          | "
             << processes[i].arrivalTime << "          | "
             << processes[i].burstTime << "        | "
             << processes[i].completionTime << "         | "
             << processes[i].waitingTime << "          | "
             << processes[i].turnaroundTime << endl;
    }

    // Calculate and display the average waiting and turnaround time
    cout << "\nAverage Waiting Time: " << (float)totalWaitingTime / n << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / n << endl;
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    Process processes[n];  // Declare array of processes

    // Take input from the user for each process
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;  // Process ID starts from 1
        cout << "Enter Arrival Time and Burst Time for Process " << i + 1 << ": ";
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime;  // Initialize remaining time
    }

    // Call SRTF scheduling function
    srtfScheduling(processes, n);

    return 0;
}

----------------------------------------------------------------------------------------------------------------------------------

//Write a program to calculate sum of n numbers using Pthreads. A list of n numbers is divided into two smaller
list of equal size, two separate threads are used to sum the sub lists.

#include <iostream>
#include <pthread.h>

using namespace std;

// Structure to pass data to threads
struct ThreadData {
    int* arr;           // Pointer to the array of numbers
    int startIdx;       // Start index for the sublist
    int endIdx;         // End index for the sublist
    int result;         // Result to store the sum of the sublist
};

// Function to be executed by each thread to calculate the sum of a sublist
void* sumSubList(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sum = 0;

    // Calculate sum for the sublist from startIdx to endIdx
    for (int i = data->startIdx; i < data->endIdx; i++) {
        sum += data->arr[i];
    }

    // Store the result in the result field of ThreadData
    data->result = sum;
    return nullptr;
}

int main() {
    int n;

    // Taking input for the number of elements
    cout << "Enter the number of elements: ";
    cin >> n;

    // Dynamically allocate memory for the list of numbers
    int* numbers = new int[n];

    // Taking input for the list of numbers
    cout << "Enter " << n << " numbers: ";
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }

    // Divide the array into two parts
    int mid = n / 2;

    // Create two ThreadData objects to store sublist information for each thread
    ThreadData data1 = {numbers, 0, mid, 0};
    ThreadData data2 = {numbers, mid, n, 0};

    pthread_t thread1, thread2;

    // Create two threads to sum the two sublists
    pthread_create(&thread1, nullptr, sumSubList, &data1);
    pthread_create(&thread2, nullptr, sumSubList, &data2);

    // Wait for both threads to complete
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    // Combine the results from both threads to get the total sum
    int totalSum = data1.result + data2.result;

    // Output the total sum
    cout << "The sum of the numbers is: " << totalSum << endl;

    // Deallocate the dynamically allocated memory
    delete[] numbers;

    return 0;
}

------------------------------------------------------------------------------------------------------------------------------------

//Write a program to implement first-fit, best-fit and worst-fit allocation strategies.

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

// Function to perform First-Fit allocation
void firstFit(vector<int> blocks, vector<int> processes) {
    vector<int> allocation(processes.size(), -1);

    for (int i = 0; i < processes.size(); i++) {
        for (int j = 0; j < blocks.size(); j++) {
            if (blocks[j] >= processes[i]) {
                allocation[i] = j;
                blocks[j] -= processes[i];
                break;
            }
        }
    }

    cout << "First-Fit Allocation:" << endl;
    for (int i = 0; i < processes.size(); i++) {
        if (allocation[i] != -1)
            cout << "Process " << i + 1 << " allocated to block " << allocation[i] + 1 << endl;
        else
            cout << "Process " << i + 1 << " not allocated." << endl;
    }
}

// Function to perform Best-Fit allocation
void bestFit(vector<int> blocks, vector<int> processes) {
    vector<int> allocation(processes.size(), -1);

    for (int i = 0; i < processes.size(); i++) {
        int bestIdx = -1;
        for (int j = 0; j < blocks.size(); j++) {
            if (blocks[j] >= processes[i]) {
                if (bestIdx == -1 || blocks[j] < blocks[bestIdx])
                    bestIdx = j;
            }
        }
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blocks[bestIdx] -= processes[i];
        }
    }

    cout << "Best-Fit Allocation:" << endl;
    for (int i = 0; i < processes.size(); i++) {
        if (allocation[i] != -1)
            cout << "Process " << i + 1 << " allocated to block " << allocation[i] + 1 << endl;
        else
            cout << "Process " << i + 1 << " not allocated." << endl;
    }
}

// Function to perform Worst-Fit allocation
void worstFit(vector<int> blocks, vector<int> processes) {
    vector<int> allocation(processes.size(), -1);

    for (int i = 0; i < processes.size(); i++) {
        int worstIdx = -1;
        for (int j = 0; j < blocks.size(); j++) {
            if (blocks[j] >= processes[i]) {
                if (worstIdx == -1 || blocks[j] > blocks[worstIdx])
                    worstIdx = j;
            }
        }
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blocks[worstIdx] -= processes[i];
        }
    }

    cout << "Worst-Fit Allocation:" << endl;
    for (int i = 0; i < processes.size(); i++) {
        if (allocation[i] != -1)
            cout << "Process " << i + 1 << " allocated to block " << allocation[i] + 1 << endl;
        else
            cout << "Process " << i + 1 << " not allocated." << endl;
    }
}

// Main function
int main() {
    vector<int> blocks = {100, 500, 200, 300, 600};
    vector<int> processes = {212, 417, 112, 426};

    cout << "Memory Blocks: ";
    for (int block : blocks)
        cout << block << " ";
    cout << endl;

    cout << "Processes: ";
    for (int process : processes)
        cout << process << " ";
    cout << endl;

    firstFit(blocks, processes);
    cout << endl;

    bestFit(blocks, processes);
    cout << endl;

    worstFit(blocks, processes);

    return 0;
}

