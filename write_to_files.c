#include <stdio.h>
#include <stdlib.h>


void writeCompressedToFile(const char *filename, const char *compressedData) {
    // Open the .huf file in write mode
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file '%s' for writing\n", filename);
        exit(1);
    }

    // Write the compressed data to the file
    fprintf(file, "%s", compressedData);

    // Close the file
    fclose(file);

    printf("\nCompressed data successfully written to '%s'\n", filename);
}

void writeDecompressedToFile(const char *filename, const char *decompressedData) {
    // Open the .txt file in write mode
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file '%s' for writing\n", filename);
        exit(1);
    }

    // Write the decompressed data (original text) to the file
    fprintf(file, "%s", decompressedData);

    // Close the file
    fclose(file);

    printf("\nDecompressed data successfully written to '%s'\n", filename);
}
