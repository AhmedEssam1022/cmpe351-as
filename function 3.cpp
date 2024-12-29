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

