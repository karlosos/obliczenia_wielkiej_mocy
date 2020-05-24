#include <iostream>
#include <random>
#include "tbb/parallel_invoke.h"
#include <tbb/mutex.h>

using namespace std;

const int size = 100;
int currentBoard[size][size];
int nextBoard[size][size];
tbb::mutex boardMutex[size][size]; // chroni dostep do nextBoard

void initializeBoard() {
  srand((unsigned)time(NULL));
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++) {
      int cell = 0;
      float p = (float) rand()/RAND_MAX;
      if (p < 0.4) {
        cell = (rand() % 2) + 1;
      }
      currentBoard[i][j] = cell; 
    }
  }
}

void flipBoard() {
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++) {
      currentBoard[i][j] = nextBoard[i][j];
      nextBoard[i][j] = 0;
    }
  }
}

void saveBoard(int iteration) {
unsigned char colorTheme[][3] = {{0, 0, 0}, {220, 30, 15}, {30, 220, 15}};
  FILE * fp;
  int i,j;
  std::string fileName = "frames/frame_" + std::to_string(iteration) + ".ppm";
  if (iteration < 10) {
  fileName = "frames/frame_0" + std::to_string(iteration) + ".ppm";
  }
  const char *filename = fileName.c_str(); 
  char *comment="# ";/* comment should start with # */
  const int MaxColorComponentValue=255;
  fp = fopen(filename,"wb"); /* b -  binary mode */
  /*write ASCII header to the file*/
  fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,size*10,size*10,MaxColorComponentValue);
  for (int i=0; i<size*10; i++)
  {
    for (int j=0; j<size*10; j++)
    {
      int cell = currentBoard[i/10][j/10];
      if (cell == 0) {
        fwrite(colorTheme[0], 1, 3, fp);
      }
      else if (cell == 1) {
        fwrite(colorTheme[1], 1, 3, fp);
      } 
      else if (cell == 2) {
        fwrite(colorTheme[2], 1, 3, fp);
      }
    }
  }
  fclose(fp);
}

void iteration(int colony) {
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++) {
      int cell = currentBoard[i][j];
      // Liczenie zywych sąsiadów
      int colony1Neighbours = 0;
      int colony2Neighbours = 0;

      for (int k=i-1; k<i+2; k++) {
        for (int l=j-1; l<j+2; l++) {
          if (k >= 0 and k < size and l >= 0 and l < size) {
            int neighbourCell = currentBoard[k][l];
            if (neighbourCell == 1) {
              colony1Neighbours++;
            } 
            else if (neighbourCell == 2) {
              colony2Neighbours++;
            } 
          }
        } 
      }

      // Wyznaczanie nowej wartosci komorki
      // int cell = currentBoard[i][j];
      int newCell = 0;
      int livingNeighbours = colony1Neighbours + colony2Neighbours;
      
      // Jezeli jest martwa i ma 3 sasiadow
      if (cell == 0 and livingNeighbours == 3) {
        if (colony1Neighbours > colony2Neighbours) {
          newCell = 1;
        } 
        else {
          newCell = 2; 
        }
      } 

      // Jezeli jest zywa i ma 2 lub 3 sasiadow to pozostaje zywa
      if (cell != 0 and (livingNeighbours == 2 or livingNeighbours == 3)) {
        newCell = cell; 
      }

      // Wpisywanie nowej komorki
      // Zakladam mutex i sprawdzam czy pole nie jest już zajęte
      if (newCell == colony) {
        boardMutex[i][j].lock(); 
        if (nextBoard[i][j] == 0) {
          nextBoard[i][j] = newCell;
        }
        boardMutex[i][j].unlock(); 
      }
    }
  }
}

int main() {
  // Inicjalizacja planszy
  initializeBoard();

  for (int i=0; i<99; i++) {
    // Reguly iteracja
    tbb::parallel_invoke([]{iteration(1);}, []{iteration(2);} );
    // Zapisywanie currentBoard do obrazka
    saveBoard(i+1);
    // Czyszczenie tablic
    flipBoard();
  }

  return 0;
}
