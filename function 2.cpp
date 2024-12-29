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
