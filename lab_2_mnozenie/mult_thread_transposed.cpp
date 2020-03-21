#include <iostream>
#include <array>
#include <thread>
#include <chrono> 

const int threadsCount = 4;

const int matrixSize = 1000;
std::array<std::array<int, matrixSize>, matrixSize> leftMatrix; 
std::array<std::array<int, matrixSize>, matrixSize> rightMatrix; 
std::array<std::array<int, matrixSize>, matrixSize> rightMatrixTransposed; 

const int outputRowCount = matrixSize;
const int outputColCount = matrixSize;
const int inputCommonSize = matrixSize;
std::array<std::array<int, outputRowCount>, outputColCount> outputMatrix;

void mult(int startIndexRow, int endIndexRow, int colCount) {
    std::cout << "I am in thread" << std::endl;
    for (int i=startIndexRow; i<endIndexRow; i++) {
        for (int j=0; j<colCount; j++) {
            int sum = 0;
            for (int k=0; k<colCount; k++) {
                sum += leftMatrix[i][k] * rightMatrixTransposed[j][k];
            }
            outputMatrix[i][j] = sum;
        }
    }
}

int main () {
    // wypelnienie macierzy danymi
    int a = 0;
    for (int i=0; i<matrixSize; i++) {
        for (int j=0; j<matrixSize; j++) {
            leftMatrix[i][j] = a;
            rightMatrix[i][j] = a++;
        } 
    }

    // Wyswietlenie macierzy
    // std::cout << "Left matrix:" << std::endl;
    // for (const auto& row: leftMatrix) {
    //     for (const auto& el: row) {
    //        std::cout << el << "\t";
    //     }
    //     std::cout << std::endl;
    // }

    // std::cout << "Right matrix:" << std::endl;
    // for (const auto& row: rightMatrix) {
    //     for (const auto& el: row) {
    //        std::cout << el << "\t";
    //     }
    //     std::cout << std::endl;
    // }

    auto start = std::chrono::high_resolution_clock::now(); 

    // Transponowanie prawej macierzy
    for (int i=0; i<matrixSize; i++) {
        for (int j=0; j<matrixSize; j++) {
            rightMatrixTransposed[i][j] = rightMatrix[j][i];
        } 
    }
    auto startMultiplication = std::chrono::high_resolution_clock::now(); 

    // Mnozenie macierzy
    int rowsPerThread = outputRowCount/threadsCount;
    std::thread multtiplicationThreads[threadsCount];

    for (int i=0; i<threadsCount; i++) {
        int startIndexRow = i*rowsPerThread;
        int endIndexRow = startIndexRow + rowsPerThread;
        if (i == threadsCount-1) {
            endIndexRow = outputColCount;
        }

        int colCount = outputColCount;
        multtiplicationThreads[i] = std::thread(mult, startIndexRow, endIndexRow, colCount);
    }

    for (int i=0; i<threadsCount; i++) {
        multtiplicationThreads[i].join();
    }

    auto stop = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 
    std::cout << "Time in ms: " << duration.count() << std::endl; 

    // Wyswietlenie macierzy wynikowej
    // std::cout << "Output matrix" << std::endl;
    // for (const auto& row: outputMatrix) {
    //     for (const auto& el: row) {
    //        std::cout << el << "\t";
    //     }
    //     std::cout << std::endl;
    // }
    return 0;
}
