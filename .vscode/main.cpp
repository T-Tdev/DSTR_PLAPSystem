#include "structures.hpp"
#include <fstream>
#include <iomanip>

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

void RiskPriorityQueue::insert(Learner l) {
    // Risk calculation based on performance indicators [cite: 45]
    l.riskScore = (l.failedAttempts * 2.0f) + (100.0f - l.totalScore) / 10.0f;
    if (l.riskScore > 7) l.recommendation = "Repeat previous topic.";
    else if (l.riskScore > 4) l.recommendation = "Attempt easier activity.";

    heap[size] = l;
    int i = size++;
    while (i > 0 && heap[(i - 1) / 2].riskScore < heap[i].riskScore) {
        swap(heap[i], heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void RiskPriorityQueue::displayHighRisk() {
    cout << "\n--- At-Risk Learner Priority List ---" << endl;
    for (int i = 0; i < size; i++) {
        cout << i + 1 << ". " << heap[i].name << " | Risk: " << fixed << setprecision(1) 
             << heap[i].riskScore << " | Rec: " << heap[i].recommendation << endl;
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
        case 5:
            // Simulation of risk data
            risk.insert({"TP01", "John", 45, 3}); 
            risk.insert({"TP02", "Sarah", 85, 0});
            risk.displayHighRisk();
            break;
        }
    } while (choice != 0);

    return 0;
}