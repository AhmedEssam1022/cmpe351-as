include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <climits>

struct Process {
    int burst;
    int arrival;
    int priority;
    int id;
    int waiting;
    int remaining;
    int done_time;
    Process* next;
};

class ProcessList {
private:
    Process* first;
    int num;

public:
    ProcessList() {
        first = nullptr;
        num = 0;
    }
