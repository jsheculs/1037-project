//
// Created by Amelia Korble on 2024-11-27.
//
#ifndef AKORBLEHUFFMAN_H
#define AKORBLEHUFFMAN_H

#endif //AKORBLEHUFFMAN_H





typedef struct {
    int index;
    unsigned int weight;
} node_t;

// The HuffmanNode definition
typedef struct HuffmanNode {
    int frequency;
    struct HuffmanNode* left;
    struct HuffmanNode* right;
    char data;
} HuffmanNode;


typedef struct QueueNode {
    HuffmanNode *node;
    struct QueueNode* next;      // Pointer to the next node
} QueueNode;
typedef struct {
    QueueNode* front;            // Pointer to the front of the queue
    QueueNode* rear;
    // Pointer to the rear of the queue
} Queue;



void countFrequencies(const char *filename, node_t nodes[], int *uniqueCount);
void priorityEnqueue(Queue *q, HuffmanNode *newNode);
HuffmanNode* priorityDequeue(Queue *q);
void initQueue(Queue *q);
void freeQueue(Queue *q);