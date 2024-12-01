#include <iostream>
#include <cstring>

using namespace std;

// Constants
const int MAX_FILES = 100;
const int BLOCK_SIZE = 1024;
const int NUM_BLOCKS = 1000;

// File structure
struct File {
    char name[50];
    int size;
    int blocks[10]; // Assuming each file can have up to 10 blocks
};

// Directory structure
File directory[MAX_FILES];

// File Allocation Table (FAT)
bool fat[NUM_BLOCKS] = {false}; // false indicates free, true indicates allocated

// Function prototypes
void createFile(const char* filename, int size);
void deleteFile(const char* filename);
void writeFile(const char* filename, const char* data);
char* readFile(const char* filename);
void listDirectory();

int main() {
    // Example usage
    createFile("file1.txt", 1024);
    createFile("file2.txt", 2048);
    listDirectory();
    writeFile("file1.txt", "Hello, World!");
    cout << "Contents of file1.txt: " << readFile("file1.txt") << endl;
    deleteFile("file1.txt");
    listDirectory();

    return 0;
}

void createFile(const char* filename, int size) {
    for (int i = 0; i < MAX_FILES; ++i) {
        if (strlen(directory[i].name) == 0) {
            strcpy(directory[i].name, filename);
            directory[i].size = size;
            int numBlocksNeeded = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
            int allocatedBlocks = 0;

            for (int j = 0; j < NUM_BLOCKS && allocatedBlocks < numBlocksNeeded; ++j) {
                if (!fat[j]) {
                    fat[j] = true;
                    directory[i].blocks[allocatedBlocks++] = j;
                }
            }

            if (allocatedBlocks < numBlocksNeeded) {
                cout << "Not enough space to create file" << endl;
                // Rollback changes if not enough space
                for (int j = 0; j < allocatedBlocks; ++j) {
                    fat[directory[i].blocks[j]] = false;
                }
                memset(directory[i].name, 0, sizeof(directory[i].name));
            } else {
                cout << "File created successfully" << endl;
            }
            return;
        }
    }
    cout << "Directory full, cannot create file" << endl;
}

void deleteFile(const char* filename) {
    for (int i = 0; i < MAX_FILES; ++i) {
        if (strcmp(directory[i].name, filename) == 0) {
            for (int j = 0; j < 10 && directory[i].blocks[j] != -1; ++j) {
                fat[directory[i].blocks[j]] = false;
            }
            memset(directory[i].name, 0, sizeof(directory[i].name));
            cout << "File deleted successfully" << endl;
            return;
        }
    }
    cout << "File not found" << endl;
}

void writeFile(const char* filename, const char* data) {
    for (int i = 0; i < MAX_FILES; ++i) {
        if (strcmp(directory[i].name, filename) == 0) {
            // Simulate writing data to blocks
            cout << "Writing data to file..." << endl;
            cout << "Data written successfully" << endl;
            return;
        }
    }
    cout << "File not found" << endl;
}

char* readFile(const char* filename) {
    static char data[BLOCK_SIZE];
    for (int i = 0; i < MAX_FILES; ++i) {
        if (strcmp(directory[i].name, filename) == 0) {
            // Simulate reading data from blocks
            cout << "Reading data from file..." << endl;
            cout << "Data read successfully" << endl;
            return data;
        }
    }
    cout << "File not found" << endl;
    return nullptr;
}

void listDirectory() {
    cout << "Directory Listing:" << endl;
    for (int i = 0; i < MAX_FILES; ++i) {
        if (strlen(directory[i].name) != 0) {
            cout << directory[i].name << " (" << directory[i].size << " bytes)" << endl;
        }
    }
}
