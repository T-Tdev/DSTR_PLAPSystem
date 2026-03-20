#include "structures.hpp"
#include <fstream>
#include <iomanip>
#include <algorithm>

// Helper Constants for UI Clarity
const string modules[] = { "CCP", "CSLLT", "DSTR", "DMPM", "WAPP" };
const string days[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };
const string times[] = { "08:00-10:00", "10:00-12:00", "12:00-14:00", "14:00-16:00", "16:00-18:00" };

// --- TASK 1 IMPLEMENTATION ---
// This is the "missing" symbol the linker is looking for
SessionQueue::SessionQueue() : front(nullptr), rear(nullptr), count(0) {
    // Leave empty or add initialization logic here [cite: 18, 59]
}
SessionQueue::~SessionQueue() {
    QueueNode* curr = front;
    while (curr != nullptr) {
        QueueNode* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
}

void SessionQueue::enqueue(Learner l) {
    if (isFull()) return;
    QueueNode* newNode = new QueueNode{ l, nullptr };
    if (!rear) front = rear = newNode;
    else { rear->next = newNode; rear = newNode; }
    count++;
}

bool SessionQueue::isStudentEnrolled(string studentID) {
    QueueNode* curr = front;
    while (curr) {
        if (curr->data.id == studentID) return true;
        curr = curr->next;
    }
    return false;
}

bool SessionQueue::dequeue(string studentID, string studentName) {
    if (isEmpty()) return false;
    QueueNode* curr = front, * prev = nullptr;
    while (curr != nullptr) {
        if (curr->data.id == studentID && curr->data.name == studentName) break;
        prev = curr; curr = curr->next;
    }
    if (curr == nullptr) return false;
    if (prev == nullptr) front = curr->next;
    else prev->next = curr->next;
    if (curr == rear) rear = prev;
    delete curr;
    count--;
    return true;
}

// --- TASK 2 IMPLEMENTATION (Stack) ---
void ActivityStack::push(Activity a) {
    StackNode* newNode = new StackNode{ a, top };
    top = newNode;
}

Activity ActivityStack::pop() {
    if (isEmpty()) return { 0, "None", "None", "None", 0 };
    StackNode* temp = top;
    Activity data = temp->data;
    top = top->next;
    delete temp;
    return data;
}

// --- TASK 3 IMPLEMENTATION (Circular Array) ---
CircularLog::CircularLog(int cap) : head(0), tail(0), size(0), capacity(cap) {
    log = new Activity[capacity];
}

void CircularLog::addLog(Activity a) {
    log[tail] = a;
    tail = (tail + 1) % capacity;
    if (size < capacity) size++;
    else head = (head + 1) % capacity; // Overwrite oldest [cite: 38]
}

void CircularLog::exportToCSV() {
    ofstream file("activity_logs.csv");
    file << "ID,Topic,Difficulty,Score\n";
    for (int i = 0; i < size; i++) {
        int idx = (head + i) % capacity;
        file << log[idx].id << "," << log[idx].topic << "," << log[idx].difficulty << "," << log[idx].score << "\n";
    }
    file.close();
    cout << "[System] Logs exported to activity_logs.csv" << endl;
}

// --- TASK 4 IMPLEMENTATION (Priority Queue/Heap) ---
RiskPriorityQueue::RiskPriorityQueue(int cap) : size(0), capacity(cap) {
    heap = new Learner[capacity];
}

RiskPriorityQueue::~RiskPriorityQueue() {
    delete[] heap;
}

bool RiskPriorityQueue::isFull() {
    return size == capacity;
}

bool RiskPriorityQueue::isEmpty() {
    return size == 0;
}

void RiskPriorityQueue::insert(Learner l) {
    if (size >= capacity) return;
    // Risk Score Calculation 
    l.riskScore = (l.failedAttempts * 2.0f) + ((100.0f - l.totalScore) / 10.0f);
    l.recommendation = (l.riskScore > 7.0f) ? "Repeat previous topic." : "Continue progress.";
    
    heap[size] = l;
    int i = size;
    size++;

    // Heapify up (max-heap by riskScore)
    while (i > 0 && heap[(i - 1) / 2].riskScore < heap[i].riskScore) {
        Learner temp = heap[i];
        heap[i] = heap[(i - 1) / 2];
        heap[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

void RiskPriorityQueue::displayHighRisk() {
    cout << "\n--- At-Risk Priority List ---" << endl;
    for (int i = 0; i < size; i++) {
        cout << i + 1 << ". " << heap[i].id << " | Risk: " << fixed << setprecision(1) << heap[i].riskScore << " | " << heap[i].recommendation << endl;
    }
}

// --- MAIN MENU ---
int main() {
    // 3D Array: 5 Modules x 5 Days x 5 Time Slots [cite: 12]
    SessionQueue schedule[5][5][5];

    int choice;
    do {
        cout << "\n==========================================";
        cout << "\n    ASIA PACIFIC UNIVERSITY - PLAPS       ";
        cout << "\n==========================================";
        cout << "\n1. Register & Join a Learning Session (Task 1)";
        cout << "\n2. View My Schedule & Quit a Session (Task 1)";
        cout << "\n3. Activities & Navigation (Task 2 & 3)";
        cout << "\n4. View At-Risk Recommendations (Task 4)";
        cout << "\n0. Exit System";
        cout << "\n------------------------------------------";
        cout << "\nPlease select an option by number: ";

        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(100, '\n'); continue;
        }

        if (choice == 1) {
            int m, d, t;
            cout << "\n---- Course Modules ----" << endl;
            for (int i = 0; i < 5; i++) cout << i + 1 << ". " << modules[i] << endl;
            cout << "Please select the module by number (1-5): "; cin >> m;
            if (m < 1 || m > 5) { cout << "\n[!] There is no such modules/sessions, back to main page." << endl; continue; }

            cout << "\n---- Available Days ----" << endl;
            for (int i = 0; i < 5; i++) cout << i + 1 << ". " << days[i] << endl;
            cout << "Please select the day by number (1-5): "; cin >> d;
            if (d < 1 || d > 5) { cout << "\n[!] There is no such modules/sessions, back to main page." << endl; continue; }

            cout << "\n---- Session Time Slots ----" << endl;
            for (int i = 0; i < 5; i++) cout << i + 1 << ". " << times[i] << endl;
            cout << "Please select the time slot by number (1-5): "; cin >> t;
            if (t < 1 || t > 5) { cout << "\n[!] There is no such modules/sessions, back to main page." << endl; continue; }

            if (schedule[m - 1][d - 1][t - 1].isFull()) {
                cout << "\n[!] Notice: This session is full. Back to main page." << endl; [cite: 23]
            } else {
                Learner l;
                cout << "\n---- Learner Registration ----" << endl;
                cout << "Enter TP Number: "; cin >> l.id;
                cout << "Enter Full Name: "; cin.ignore(); getline(cin, l.name);
                cout << "Enter Intake Course: "; getline(cin, l.intake);
                schedule[m - 1][d - 1][t - 1].enqueue(l);
                cout << "\n[System] Successfully registered for " << modules[m - 1] << "!" << endl;
            }
        }
        else if (choice == 2) {
            string sID, sName;
            cout << "\n---- Verification ----" << endl;
            cout << "Please enter your TP Number: "; cin >> sID;
            cout << "Please enter your Full Name: "; cin.ignore(); getline(cin, sName);

            bool registered[5] = { false };
            for (int m = 0; m < 5; m++)
                for (int d = 0; d < 5; d++)
                    for (int t = 0; t < 5; t++)
                        if (schedule[m][d][t].isStudentEnrolled(sID)) registered[m] = true;

            cout << "\n---- Your Registered Modules ----" << endl;
            for (int i = 0; i < 5; i++) if (registered[i]) cout << i + 1 << ". " << modules[i] << endl;
            
            cout << "Please select the module by number: ";
            int mC; cin >> mC;
            if (mC < 1 || mC > 5 || !registered[mC - 1]) {
                cout << "\n[!] There is no such modules/sessions, back to main page." << endl; continue;
            }

            int mIdx = mC - 1;
            cout << "\n---- Schedule for " << modules[mIdx] << " ----" << endl;
            cout << left << setw(15) << "Time Slot" << "| Mon | Tue | Wed | Thu | Fri | ID" << endl;
            for (int t = 0; t < 5; t++) {
                cout << left << setw(15) << times[t];
                for (int d = 0; d < 5; d++) cout << (schedule[mIdx][d][t].isStudentEnrolled(sID) ? "| YES " : "|  -  ");
                cout << "| [" << t + 1 << "]" << endl;
            }

            cout << "\n1. Select a Timeslot to Quit\n2. Back to Main Page\nChoice: ";
            int sub; cin >> sub;
            if (sub == 1) {
                int qD, qT;
                cout << "Select Day (1-5): "; cin >> qD;
                cout << "Select Time Slot (1-5): "; cin >> qT;
                if (schedule[mIdx][qD - 1][qT - 1].dequeue(sID, sName)) cout << "[System] Successfully withdrawn." << endl;
                else cout << "[!] Registration not found." << endl;
            }
            break;
        }
        case 3: { // Task 2 & 3 Placeholder
            Activity act = { 101, "Data Structures", "Hard", 85 };
            navigation.push(act);
            history.addLog(act);
            cout << "[Task 2] Activity Added to Stack & [Task 3] Logged." << endl;
            break;
        }
        case 4: { // Task 4 Placeholder
            risk.insert({ "TP01", "John", "CS", 45, 3 });
            risk.displayHighRisk();
            break;
        }
        }
    } while (choice != 0);

    return 0;
}