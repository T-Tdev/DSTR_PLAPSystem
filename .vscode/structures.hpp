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
    string id;
    string name;
    int totalScore = 0;
    int failedAttempts = 0;
    float riskScore = 0.0f;
    string recommendation = "No issues detected.";
};

// TASK 1: Linear Queue (Linked List)
struct QueueNode {
    Learner data;
    QueueNode* next;
};

class SessionQueue {
private:
    QueueNode *front, *rear;
    int count, capacity;
public:
    SessionQueue(int cap) : front(nullptr), rear(nullptr), count(0), capacity(cap) {}
    bool isFull() { return count >= capacity; }
    bool isEmpty() { return count == 0; }
    void enqueue(Learner l);
    void dequeue();
    void displayAll();
};

// TASK 2: Stack (Linked List)
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

// TASK 3: Circular Queue (Array-based)
class CircularLog {
private:
    Activity* log;
    int head, tail, size, capacity;
public:
    CircularLog(int cap);
    void addLog(Activity a);
    void displayLogs(string filterID = "");
    void exportToCSV();
};

// TASK 4: Priority Queue (Max-Heap)
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