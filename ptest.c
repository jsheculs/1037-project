#include <stdio.h>
#include <stdlib.h>
#include "compress_and_decompress.h"
#include "write_to_files.h"

int main(int argc, char **argv) {
    // File name for testing
    if (argc != 3) {
       return 0;
    }
    const char *filename = argv[1];
    const char *compressedFile = argv[2];

    // Step 1: Create a queue and initialize it
    Queue* q = (Queue*)malloc(sizeof(Queue));  // Allocate memory for the queue
    if (q == NULL) {
        printf("Memory allocation failed for queue.\n");
        return -1;
    }
    initQueue(q);  // Initialize the queue

    // Step 2: Count frequencies and fill the nodes array
    node_t nodes[256];
    int uniqueCharCount = 0;
    countFrequencies(filename, nodes, &uniqueCharCount);  // Use the filename

    // Step 3: Build the Huffman tree
    for (int i = 0; i < uniqueCharCount; i++) {
        // Create a leaf node for each unique character and its frequency
        HuffmanNode* node = createLeafNode(nodes[i].index, nodes[i].weight);
        // Enqueue it into the priority queue
        priorityEnqueue(q, node);
    }

    // Step 4: Build the Huffman Tree
    HuffmanNode* root = buildHuffmanTree(q);

    // Step 5: Generate Huffman codes
    char codes[256][MAX] = {0};
    HuffmanCodes(root, codes);

    // Step 6: Compress the file using the generated Huffman codes
    char compressed[1024] = {0};
    compress(filename, compressed, root, codes);
    writeCompressedToFile(compressedFile,compressed);

    // Step 7: Print the frequency table
    printf("Frequency Table:\n");
    for (int i = 0; i < uniqueCharCount; i++) {
        printf("'%c': %d\n", nodes[i].index, nodes[i].weight);
    }

    // Step 8: Print the Huffman codes for each character
    printf("\nHuffman Codes:\n");
    for (int i = 0; i < uniqueCharCount; i++) {
        printf("'%c' Code: %s\n", nodes[i].index, codes[(unsigned char)nodes[i].index]);
    }

    // Step 9: Print the compressed data
    printf("\nCompressed Data (in binary):\n");
    printf("%s\n", compressed);

    // Step 10: Decompress the data
    char decompressed[1024] = {0};
    decompress(compressed, decompressed, root);
    writeDecompressedToFile("decompressed.txt", decompressed);

    // Step 11: Print the decompressed data
    printf("\nDecompressed Data:\n");
    printf("%s\n", decompressed);

    // Free the allocated memory
    freeQueue(q);
    freeHuffmanTree(root);

    return 0;
}