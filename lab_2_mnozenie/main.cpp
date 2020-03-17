#include <iostream>
#include <array>

int main () {
    std::cout << "Hello world" << std::endl;
    const int matrixSize = 2;
    std::array<std::array<int, matrixSize>, matrixSize> leftMatrix; 
    std::array<std::array<int, matrixSize>, matrixSize> rightMatrix; 

    // wypelnienie macierzy danymi
    int a = 0;
    for (int i=0; i<matrixSize; i++) {
        for (int j=0; j<matrixSize; j++) {
            leftMatrix[i][j] = a;
            rightMatrix[i][j] = a++;
        } 
    }

    // Wyswietlenie macierzy
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

    // Mnozenie macierzy
    const int outputRowCount = matrixSize;
    const int outputColCount = matrixSize;
    const int inputCommonSize = matrixSize;
    std::array<std::array<int, outputRowCount>, outputColCount> outputMatrix;
    for (int i=0; i<outputRowCount; i++) {
        for (int j=0; j<outputColCount; j++) {
            int sum = 0;
            for (int k=0; k<inputCommonSize; k++) {
                sum += leftMatrix[i][k] * rightMatrix[k][j];
            }
            outputMatrix[i][j] = sum;
        }
    }

    // Wyswietlenie macierzy wynikowej
    std::cout << "Output matrix" << std::endl;
    for (const auto& row: outputMatrix) {
        for (const auto& el: row) {
           std::cout << el << "\t";
        }
        std::cout << std::endl;
    }
    return 0;
}
