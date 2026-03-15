#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <iostream>
#include <string>

using namespace std;

// Core Entities
struct Activity {
    int id;
    string topic;
    string difficulty;
    int score;
};

struct Learner {
    string id;              // TP Number
    string name;
    string intake;          // Intake/Course
    int totalScore = 0;
    int failedAttempts = 0;
    float riskScore = 0.0f;
    string recommendation = "No issues detected.";
};

// TASK 1: Session Management (Linked List Queue)
struct QueueNode {
    Learner data;
    QueueNode* next;
};

class SessionQueue {
private:
    QueueNode *front, *rear;
    int count;
    const int capacity = 50; // Requirement: Max capacity of 50
public:
    SessionQueue();
    ~SessionQueue();
    bool isFull() { return count >= capacity; }
    bool isEmpty() { return count == 0; }
    int getCount() { return count; }
    void enqueue(Learner l);
    bool dequeue(string studentID, string studentName); 
    bool isStudentEnrolled(string studentID); 
};

// TASK 2: Activity Navigation (Linked List Stack)
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

// TASK 3: Activity Logging (Array-based Circular Queue)
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

// TASK 4: Risk Prioritization (Max-Heap Priority Queue)
class RiskPriorityQueue {
private:
    Learner* heap;
    int size, capacity;
public:
    RiskPriorityQueue(int cap);
    ~RiskPriorityQueue();
    bool isFull();
    bool isEmpty();
    void insert(Learner l);
    void displayHighRisk();
    void exportRiskCSV();
};

#endif