// Karol Dzialowski 2B 39259
#include <iostream>
#include <array>
#include <thread>
#include <chrono> 

// These need to be set 
bool isThreading;
bool isTranspose;
int threadsCount;

const int matrixSize = 1000;
std::array<std::array<int, matrixSize>, matrixSize> leftMatrix; 
std::array<std::array<int, matrixSize>, matrixSize> rightMatrix; 
std::array<std::array<int, matrixSize>, matrixSize> rightMatrixTransposed; 

const int inputCommonSize = matrixSize;
std::array<std::array<int, matrixSize>, matrixSize> outputMatrix;

void mult(int startIndexRow, int endIndexRow) {
    for (int i=startIndexRow; i<endIndexRow; i++) {
        for (int j=0; j<matrixSize; j++) {
            int sum = 0;
            for (int k=0; k<matrixSize; k++) {
                sum += leftMatrix[i][k] * rightMatrix[k][j];
            }
            outputMatrix[i][j] = sum;
        }
    }
}

void mult_transposed(int startIndexRow, int endIndexRow) {
    for (int i=startIndexRow; i<endIndexRow; i++) {
        for (int j=0; j<matrixSize; j++) {
            int sum = 0;
            for (int k=0; k<matrixSize; k++) {
                sum += leftMatrix[i][k] * rightMatrixTransposed[j][k];
            }
            outputMatrix[i][j] = sum;
        }
    }
}

void mult_sequence() {
    for (int i=0; i<matrixSize; i++) {
        for (int j=0; j<matrixSize; j++) {
            int sum = 0;
            for (int k=0; k<inputCommonSize; k++) {
                if (!isTranspose)
                    sum += leftMatrix[i][k] * rightMatrix[k][j];
                else
                    sum += leftMatrix[i][k] * rightMatrix[j][k];
            }
            outputMatrix[i][j] = sum;
        }
    }
}

void clean_matrices() {
    int a = 0;
    for (int i=0; i<matrixSize; i++) {
        for (int j=0; j<matrixSize; j++) {
            leftMatrix[i][j] = a;
            rightMatrix[i][j] = a++;
            rightMatrixTransposed[i][0] = 0;
            outputMatrix[i][j] = 0;
        } 
    }
}

void transpose_right_matrix() {
    for (int i=0; i<matrixSize; i++) {
        for (int j=0; j<matrixSize; j++) {
            rightMatrixTransposed[i][j] = rightMatrix[j][i];
        } 
    }
}

void print_input_matrices() {
    std::cout << "Left matrix:" << std::endl;
    for (const auto& row: leftMatrix) {
        for (const auto& el: row) {
           std::cout << el << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << "Right matrix:" << std::endl;
    for (const auto& row: rightMatrix) {
        for (const auto& el: row) {
           std::cout << el << "\t";
        }
        std::cout << std::endl;
    }
}

void print_output_matrices() {
    std::cout << "Output matrix" << std::endl;
    for (const auto& row: outputMatrix) {
        for (const auto& el: row) {
           std::cout << el << "\t";
        }
        std::cout << std::endl;
    }
}

void experiment() {
    // wypelnienie macierzy danymi
    clean_matrices();

    // print_input_matrices();

    auto start = std::chrono::high_resolution_clock::now(); 
    if (isTranspose) {
        transpose_right_matrix();
    }

    if (isThreading) {
        int rowsPerThread = matrixSize/threadsCount;
        std::thread multtiplicationThreads[threadsCount];

        for (int i=0; i<threadsCount; i++) {
            int startIndexRow = i*rowsPerThread;
            int endIndexRow = startIndexRow + rowsPerThread;
            if (i == threadsCount-1) {
                endIndexRow = matrixSize;
            }
            if (!isTranspose) {
                multtiplicationThreads[i] = std::thread(mult, startIndexRow, endIndexRow);
            }
            else {
                multtiplicationThreads[i] = std::thread(mult_transposed, startIndexRow, endIndexRow);
            }
            
        }

        for (int i=0; i<threadsCount; i++) {
            multtiplicationThreads[i].join();
        }
    } else {
        mult_sequence();
    }

    auto stop = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); 
    std::cout << duration.count() << std::endl; 
    
    // print_output_matrices();
}

int main () {
    std::cout << "Sequence: \t\t"; 
    isTranspose = false;
    isThreading = false;
    experiment();

    std::cout << "Sequence transposed: \t";
    isTranspose = true;
    isThreading = false;
    experiment();

    std::cout << "2 Threads: \t\t";
    isTranspose = false;
    isThreading = true;
    threadsCount = 2;
    experiment();

    std::cout << "2 Threads transposed: \t";
    isTranspose = true;
    isThreading = true;
    threadsCount = 2;
    experiment();

    std::cout << "3 Threads: \t\t";
    isTranspose = false;
    isThreading = true;
    threadsCount = 3;
    experiment();

    std::cout << "3 Threads transposed: \t";
    isTranspose = true;
    isThreading = true;
    threadsCount = 3;
    experiment();

    std::cout << "4 Threads: \t\t";
    isTranspose = false;
    isThreading = true;
    threadsCount = 4;
    experiment();

    std::cout << "4 Threads transposed: \t";
    isTranspose = true;
    isThreading = true;
    threadsCount = 4;
    experiment();
    return 0;
}
