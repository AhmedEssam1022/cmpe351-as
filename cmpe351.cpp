#include <iostream>
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

    void add(int b, int a, int p, int i) {
        Process* newp = new Process();
        newp->burst = b;
        newp->arrival = a;
        newp->priority = p;
        newp->id = i;
        newp->waiting = 0;
        newp->remaining = b;
        newp->done_time = 0;
        newp->next = nullptr;

        if (!first) {
            first = newp;
        } else {
            Process* temp = first;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newp;
        }
        num++;
    }

    ProcessList* copy() {
        ProcessList* newlist = new ProcessList();
        Process* temp = first;
        while (temp) {
            newlist->add(temp->burst, temp->arrival, temp->priority, temp->id);
            temp = temp->next;
        }
        return newlist;
    }

    float avg_wait() {
        if (!first) return 0;
        float total = 0;
        Process* temp = first;
        while (temp) {
            total += temp->waiting;
            temp = temp->next;
        }
        return total / num;
    }

    void fcfs() {
        if (!first) return;
        Process* curr = first;
        int time = 0;
        while (curr) {
            if (time < curr->arrival)
                time = curr->arrival;
            curr->waiting = time - curr->arrival;
            time += curr->burst;
            curr = curr->next;
        }
    }

    void sjf() {
        if (!first) return;
        int time = 0;
        int done = 0;
        Process* orig = first;
        while (orig) {
            orig->waiting = 0;
            orig = orig->next;
        }
        
        while (done != num) {
            int minb = INT_MAX;
            Process* shortest = nullptr;
            orig = first;
            while (orig) {
                if (orig->burst < minb && orig->arrival <= time && orig->done_time == 0) {
                    minb = orig->burst;
                    shortest = orig;
                }
                orig = orig->next;
            }
            if (!shortest) {
                time++;
                continue;
            }
            shortest->waiting = time - shortest->arrival;
            time += shortest->burst;
            shortest->done_time = time;
            done++;
        }
    }

    void sjf_p() {
        if (!first) return;
        Process* temp = first;
        int time = 0;
        int done = 0;
        while (temp) {
            temp->remaining = temp->burst;
            temp->done_time = 0;
            temp = temp->next;
        }
        
        while (done != num) {
            Process* shortest = nullptr;
            int minb = INT_MAX;
            temp = first;
            while (temp) {
                if (temp->remaining > 0 && temp->arrival <= time && temp->remaining < minb) {
                    minb = temp->remaining;
                    shortest = temp;
                }
                temp = temp->next;
            }
            if (!shortest) {
                time++;
                continue;
            }
            shortest->remaining--;
            if (shortest->remaining == 0) {
                done++;
                shortest->done_time = time + 1;
                shortest->waiting = shortest->done_time - shortest->arrival - shortest->burst;
            }
            time++;
        }
    }

    void priority() {
        if (!first) return;
        int time = 0;
        int done = 0;
        Process* orig = first;
        while (done != num) {
            int highp = INT_MAX;
            Process* sel = nullptr;
            orig = first;
            while (orig) {
                if (orig->priority < highp && orig->arrival <= time && orig->done_time == 0) {
                    highp = orig->priority;
                    sel = orig;
                }
                orig = orig->next;
            }
            if (!sel) {
                time++;
                continue;
            }
            sel->waiting = time - sel->arrival;
            time += sel->burst;
            sel->done_time = time;
            done++;
        }
    }

    void priority_p() {
        if (!first) return;
        Process* temp = first;
        int time = 0;
        int done = 0;
        while (temp) {
            temp->remaining = temp->burst;
            temp->done_time = 0;
            temp = temp->next;
        }
        
        while (done != num) {
            Process* highest = nullptr;
            int highp = INT_MAX;
            temp = first;
            while (temp) {
                if (temp->remaining > 0 && temp->arrival <= time && temp->priority < highp) {
                    highp = temp->priority;
                    highest = temp;
                }
                temp = temp->next;
            }
            if (!highest) {
                time++;
                continue;
            }
            highest->remaining--;
            if (highest->remaining == 0) {
                done++;
                highest->done_time = time + 1;
                highest->waiting = highest->done_time - highest->arrival - highest->burst;
            }
            time++;
        }
    }

    void rr(int q) {
        if (!first) return;
        Process* temp = first;
        int time = 0;
        while (temp) {
            temp->remaining = temp->burst;
            temp->waiting = 0;
            temp = temp->next;
        }
        bool done = false;
        while (!done) {
            done = true;
            temp = first;
            while (temp) {
                if (temp->remaining > 0 && temp->arrival <= time) {
                    done = false;
                    if (temp->remaining > q) {
                        time += q;
                        temp->remaining -= q;
                    } else {
                        time += temp->remaining;
                        temp->waiting = time - temp->arrival - temp->burst;
                        temp->remaining = 0;
                    }
                }
                temp = temp->next;
            }
        }
    }

    int get_wait(int id) {
        Process* temp = first;
        while (temp) {
            if (temp->id == id)
                return temp->waiting;
            temp = temp->next;
        }
        return 0;
    }

    int size() { return num; }
};

class Scheduler {
private:
    ProcessList* plist;
    std::string inf;
    std::string outf;
    int quantum;

public:
    Scheduler(const std::string& in, const std::string& out, int q) {
        inf = in;
        outf = out;
        quantum = q;
        plist = new ProcessList();
    }

    void read() {
        std::ifstream file(inf);
        std::string line;
        int id = 0;
        while (std::getline(file, line)) {
            int b, a, p;
            sscanf(line.c_str(), "%d:%d:%d", &b, &a, &p);
            plist->add(b, a, p, id++);
        }
        file.close();
    }

    void write() {
        std::ofstream file(outf);
        for (int alg = 1; alg <= 6; alg++) {
            ProcessList* temp = plist->copy();
            switch (alg) {
                case 1: temp->fcfs(); break;
                case 2: temp->sjf(); break;
                case 3: temp->sjf_p(); break;
                case 4: temp->priority(); break;
                case 5: temp->priority_p(); break;
                case 6: temp->rr(quantum); break;
            }
            file << alg;
            for (int i = 0; i < temp->size(); i++) {
                file << ":" << temp->get_wait(i);
            }
            file << ":" << temp->avg_wait() << std::endl;
            delete temp;
        }
        file.close();
    }
};

int main(int argc, char* argv[]) {
    std::string inf, outf;
    int quantum = 0;
    
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-t") == 0)
            quantum = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-f") == 0)
            inf = argv[i + 1];
        else if (strcmp(argv[i], "-o") == 0)
            outf = argv[i + 1];
    }
    
    if (inf.empty() || outf.empty() || quantum == 0) {
        std::cout << "Usage: ./cpe351 -t <quantum> -f <input_file> -o <output_file>\n";
        return 1;
    }
    
    Scheduler s(inf, outf, quantum);
    s.read();
    s.write();
    
    return 0;
}