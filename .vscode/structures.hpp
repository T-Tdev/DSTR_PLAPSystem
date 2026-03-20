#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <iostream>
#include <string>

using namespace std;

// Core Entities
struct Activity {
    int id;
    string studentID;   // for task 3 (add to link activty to a another learner)
    string topic;
    string difficulty;
    int score;
};

struct Learner {
    string id;              // TP Number [cite: 12]
    string name;
    string intake;          
    int totalScore = 0;
    int failedAttempts = 0;
    float riskScore = 0.0f;
    string recommendation = "No issues detected."; [cite: 48]
};

// TASK 1: Linear Queue using Linked List [cite: 8, 18]
struct QueueNode {
    Learner data;
    QueueNode* next;
};

class SessionQueue {
private:
    QueueNode *front, *rear;
    int count;
    const int capacity = 50; // Requirement: Max capacity of 50 per session [cite: 23]
public:
    SessionQueue();
    ~SessionQueue(); 
    bool isFull() { return count >= capacity; } [cite: 23]
    bool isEmpty() { return count == 0; }
    int getCount() { return count; }
    void enqueue(Learner l); [cite: 12, 21]
    bool dequeue(string studentID, string studentName); [cite: 24]
    bool isStudentEnrolled(string studentID); 
};

// TASK 2: Activity Navigation using Linked List Stack [cite: 8, 26]
struct StackNode {
    Activity data;
    StackNode* next;
};

class ActivityStack {
private:
    StackNode* top;
public:
    ActivityStack() : top(nullptr) {}
    void push(Activity a);
    Activity pop();
    bool isEmpty() { return top == nullptr; }
};

// TASK 3: Activity Logging using Dynamic Array Circular Queue [cite: 8, 34]
class CircularLog {
private:
    Activity* log;
    int head, tail, size, capacity;
public:
    CircularLog(int cap);
    ~CircularLog() { delete[] log; }
    void addLog(Activity a);
    void exportToCSV();
};

// TASK 4: Risk Prioritization using Max-Heap [cite: 8, 42]
class RiskPriorityQueue {
private:
    Learner* heap;
    int size, capacity;
public:
    RiskPriorityQueue(int cap);
    ~RiskPriorityQueue() { delete[] heap; }
    void insert(Learner l);
    void displayHighRisk();
};

#endif