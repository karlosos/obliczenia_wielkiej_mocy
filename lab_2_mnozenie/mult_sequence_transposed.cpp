#include <iostream>
#include <array>
#include <chrono> 

const int matrixSize = 1000;
std::array<std::array<int, matrixSize>, matrixSize> leftMatrix; 
std::array<std::array<int, matrixSize>, matrixSize> rightMatrix; 
std::array<std::array<int, matrixSize>, matrixSize> rightMatrixTransposed; 

const int outputRowCount = matrixSize;
const int outputColCount = matrixSize;
const int inputCommonSize = matrixSize;
std::array<std::array<int, outputRowCount>, outputColCount> outputMatrix;

void mult() {
    // Mnozenie macierzy
    for (int i=0; i<outputRowCount; i++) {
        for (int j=0; j<outputColCount; j++) {
            int sum = 0;
            for (int k=0; k<inputCommonSize; k++) {
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
    mult();
    auto stop = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 
    auto durationMultiplication = std::chrono::duration_cast<std::chrono::microseconds>(stop - startMultiplication); 
    std::cout << "Time with transpose in ms: " << duration.count() << std::endl; 
    std::cout << "Time multiplication in ms: " << durationMultiplication.count() << std::endl; 

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
