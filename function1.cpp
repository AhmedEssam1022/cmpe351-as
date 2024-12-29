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
