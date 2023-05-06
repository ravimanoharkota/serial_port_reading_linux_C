//https://stackoverflow.com/questions/31388934/writing-to-reading-from-file-using-pointers-c

#include <stdio.h>

int database_record_data(char *buffer, int buffersize)
{
    FILE *fTest;
    int *testPtr;
    int x = 10;
    char const* file = "test.data"; // Using .data instead of .c

    testPtr = &x;

    int i;

    // Write the data.
    if ((fTest = fopen(file, "wb")) == NULL) {
        printf("Error!");
    }
    for (i = 0; i < 5; i++) {
        fwrite(testPtr, sizeof(int), 1, fTest);
        *testPtr += 1;
    }

    fclose(fTest);

    // Read the data.
    if ((fTest = fopen(file, "rb")) == NULL) {
        printf("Error!");
    }

    for (i = 0; i < 5; i++) {
        fread(testPtr, sizeof(int), 1, fTest);
        printf("%d", *testPtr);
    }

    fclose(fTest);
}
