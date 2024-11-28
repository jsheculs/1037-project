//
// Created by Cameron Brewster on 2024-11-20.
//
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "compress_and_decompress.h"
#include <string.h>


HuffmanNode *createLeafNode(char data, int frequency) {
    HuffmanNode * newNode = (HuffmanNode*)malloc(sizeof(HuffmanNode)); // Allocating memory
    if(newNode == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE); //Edge case if memory allocation fails
    }

    newNode -> frequency = frequency;
    newNode -> data = data;
    newNode -> left = NULL;
    newNode -> right =NULL; // Initializing leaf node
    return newNode;

}

HuffmanNode *createInternalNodes(HuffmanNode *left, HuffmanNode *right) {
    HuffmanNode *newNode = (HuffmanNode*)malloc(sizeof(HuffmanNode)); // allocating memory for internal node
    if(newNode == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);  //Edge case if memory allocation fails
    }
    newNode -> frequency = left->frequency + right->frequency; // Setting the frequency to the sum of both its child nodes
    newNode -> data = '\0'; // Internal nodes do not hold a character
    newNode -> left = left; // Point to children
    newNode -> right = right;

    return newNode;
}



HuffmanNode* buildHuffmanTree(Queue* queue) {
    while (queue->front != NULL && queue->front->next != NULL) {  // While more than one node in the queue

        // Dequeue the two nodes with the smallest frequencies
        HuffmanNode* left = priorityDequeue(queue);
        HuffmanNode* right = priorityDequeue(queue);

        // Create a new internal node
        HuffmanNode* internalNode = createInternalNodes(left, right);

        // Enqueue the new internal node back into the queue
        priorityEnqueue(queue, internalNode);
    }

    // The final node left in the queue is the root of the Huffman tree
    return priorityDequeue(queue);
}

    // Only one node should remain in the queue, which is the root of the Huffman tree

void freeHuffmanTree(HuffmanNode* root) {
    if(root == NULL) {
        return; // Edge case if tree DNE
    }
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right); // Recursive calls

    free(root);
}
void getCode(HuffmanNode *root, char codes[256][MAX], char* currentCode, int depth) {
    if (root == NULL) {
        return;
    }

    // Base case: if the node is a leaf node (no children)
    if (root->left == NULL && root->right == NULL) {
        currentCode[depth] = '\0';  // Null-terminate the code string
        strcpy(codes[(unsigned char)root->data], currentCode);  // Store the code for this character
        return;
    }

    // Traverse the left subtree
    currentCode[depth] = '0';
    getCode(root->left, codes, currentCode, depth + 1);

    // Traverse the right subtree
    currentCode[depth] = '1';
    getCode(root->right, codes, currentCode, depth + 1);
}

void HuffmanCodes(HuffmanNode *root, char codes[256][MAX]) {
    char currentCode[MAX] = {0};  // Initialize currentCode array to store the current code being generated

    // Special case for when there is only one character (tree with only one node)
    if (root != NULL && root->left == NULL && root->right == NULL) {
        currentCode[0] = '0';  // Assign the code '0' for the single character
        codes[(unsigned char)root->data][0] = '0';
        codes[(unsigned char)root->data][1] = '\0';  // Null-terminate the string
        //printf("Character: '%c', Code: %s\n", root->data, currentCode);  // Debug print
    } else {
        getCode(root, codes, currentCode, 0);  // Call getCode to generate the codes
    }
}
void compress(const char *filename, char *compressed, HuffmanNode *root, char codes[256][MAX]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", filename);
        exit(1);
    }

    int j = 0;  // Index for storing bits in the compressed array
    int c;
    while ((c = fgetc(file)) != EOF) {
        // Get the Huffman code for the current character
        char *code = codes[(unsigned char)c];  // Get the Huffman code for the current character

        // Append the Huffman code to the compressed array bit by bit
        for (int k = 0; code[k] != '\0'; k++) {
            compressed[j++] = code[k];  // Store each bit as a character ('0' or '1') in the compressed array
        }
    }

    // Ensure the string is null-terminated after the last bit (not adding extra bits)
    compressed[j] = '\0';  // Null-terminate the compressed data to indicate the end of the bit string

    fclose(file);
}

void decompress(const char *compressed, char *decompressed, HuffmanNode *root) {
    if (root == NULL) {
        printf("Error: Huffman Tree is empty!\n");
        return;
    }

    // Check if the tree has only one node
    if (root->left == NULL && root->right == NULL) {
        // Handle the special case for a single character tree
        int index = 0;
        while (compressed[index] != '\0') {
            decompressed[index] = root->data;  // Just append the character
            index++;
        }
        decompressed[index] = '\0';  // Null-terminate the string
        printf("Decompressed Data: %s\n", decompressed);
        return;
    }

    // General case: Traverse the tree
    int index = 0;  // Pointer to the current position in the compressed string
    int decompressedIndex = 0;  // Pointer to the current position in the decompressed string
    HuffmanNode *currentNode = root;  // Start from the root of the tree

    // Traverse the compressed binary string
    while (compressed[index] != '\0') {
        // Traverse the tree based on the current bit (0 or 1)
        if (compressed[index] == '0') {
            currentNode = currentNode->left;  // Go to left child
        } else if (compressed[index] == '1') {
            currentNode = currentNode->right;  // Go to right child
        } else {
            printf("Error: Invalid compressed data!\n");
            return;
        }

        // If a leaf node is reached (i.e., we have a character)
        if (currentNode->left == NULL && currentNode->right == NULL) {
            decompressed[decompressedIndex++] = currentNode->data;  // Add the character to the decompressed string
            currentNode = root;  // Reset to the root of the tree to decode the next character
        }

        index++;  // Move to the next bit in the compressed string
    }

    decompressed[decompressedIndex] = '\0';  // Null-terminate the decompressed string
}