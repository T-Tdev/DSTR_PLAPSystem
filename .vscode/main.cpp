#include "structures.hpp"
#include <fstream>
#include <iomanip>
#include <algorithm>

// --- TASK 1 IMPLEMENTATION ---
void SessionQueue::enqueue(Learner l) {
    if (isFull()) { cout << "Error: Session capacity full!" << endl; return; }
    QueueNode* newNode = new QueueNode{l, nullptr};
    if (!rear) front = rear = newNode;
    else { rear->next = newNode; rear = newNode; }
    count++;
}

void SessionQueue::dequeue() {
    if (isEmpty()) return;
    QueueNode* temp = front;
    front = front->next;
    if (!front) rear = nullptr;
    delete temp;
    count--;
}

void SessionQueue::displayAll() {
    QueueNode* curr = front;
    cout << "\n--- Active Session List ---" << endl;
    while(curr) {
        cout << "ID: " << curr->data.id << " | Name: " << curr->data.name << endl;
        curr = curr->next;
    }
}

// --- TASK 2 IMPLEMENTATION ---
void ActivityStack::push(Activity a) {
    StackNode* newNode = new StackNode{a, top};
    top = newNode;
}

Activity ActivityStack::pop() {
    if (isEmpty()) return {0, "None", "None", 0};
    StackNode* temp = top;
    Activity data = temp->data;
    top = top->next;
    delete temp;
    return data;
}

// --- TASK 3 IMPLEMENTATION ---
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
    cout << "System: Logs exported to activity_logs.csv" << endl;
}

// --- TASK 4 IMPLEMENTATION ---
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
    if (isFull()) {
        cout << "Error: Risk priority queue is full!" << endl;
        return;
    }

    // Calculate learner risk score
    l.riskScore = (l.failedAttempts * 2.0f) + ((100.0f - l.totalScore) / 10.0f);

    // Recommendation based on risk level
    if (l.riskScore > 7.0f)
        l.recommendation = "Repeat previous topic.";
    else if (l.riskScore > 4.0f)
        l.recommendation = "Attempt easier activity.";
    else
        l.recommendation = "Continue current progress.";

    // Insert at end of heap
    heap[size] = l;
    int i = size;
    size++;

    // Heapify up (max-heap by riskScore)
    while (i > 0 && heap[(i - 1) / 2].riskScore < heap[i].riskScore) {
        swap(heap[i], heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void RiskPriorityQueue::displayHighRisk() {
    if (isEmpty()) {
        cout << "\nNo at-risk learners recorded yet." << endl;
        return;
    }

    cout << "\n--- At-Risk Learner Priority List ---" << endl;
    for (int i = 0; i < size; i++) {
        cout << i + 1 << ". "
             << "ID: " << heap[i].id
             << " | Name: " << heap[i].name
             << " | Total Score: " << heap[i].totalScore
             << " | Failed Attempts: " << heap[i].failedAttempts
             << " | Risk Score: " << fixed << setprecision(1) << heap[i].riskScore
             << " | Recommendation: " << heap[i].recommendation
             << endl;
    }
}

// --- MAIN UI LOOP ---
int main() {
    SessionQueue session(5);      // Task 1
    ActivityStack navigation;     // Task 2
    CircularLog history(10);      // Task 3
    RiskPriorityQueue risk(10);   // Task 4

    int choice;
    do {
        cout << "\n==== APU PLAPS PROTOTYPE ====";
        cout << "\n1. Register & Join Session (Task 1)";
        cout << "\n2. Move to Next Activity (Task 2)";
        cout << "\n3. Undo/Backtrack Activity (Task 2)";
        cout << "\n4. Export Activity Logs (Task 3)";
        cout << "\n5. View At-Risk Learners (Task 4)";
        cout << "\n0. Exit System";
        cout << "\nSelection: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            Learner l;
            cout << "Enter Student ID & Name: "; cin >> l.id >> l.name;
            session.enqueue(l);
            break;
        }
        case 2: {
            Activity a = {101, "Arrays", "Medium", 65};
            navigation.push(a);
            history.addLog(a);
            cout << "Started Activity: " << a.topic << endl;
            break;
        }
        case 3: {
            Activity back = navigation.pop();
            if (back.id != 0) cout << "Backtracked from: " << back.topic << endl;
            else cout << "No activities to undo." << endl;
            break;
        }
        case 4:
            history.exportToCSV();
            break;
        case 5: {
            RiskPriorityQueue risk(10);

            risk.insert({"TP01", "John", 45, 3});
            risk.insert({"TP02", "Sarah", 85, 0});
            risk.insert({"TP03", "Ali", 60, 2});
            risk.insert({"TP04", "Mei", 30, 4});

            risk.displayHighRisk();
            break;
        }
    }
    } while (choice != 0);

    return 0;
}
