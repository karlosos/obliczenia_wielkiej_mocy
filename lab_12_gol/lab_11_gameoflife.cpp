#include <iostream>
#include <random>

using namespace std;

const int size = 100;
int currentBoard[size][size];
int nextBoard[size][size];

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

void clearBoard() {
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++) {
      nextBoard[i][j]=0;
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
  std::string fileName = "frame_" + std::to_string(iteration) + ".ppm";
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

void iteration() {
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++) {
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

      int cell = currentBoard[i][j];
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

      // JEzeli jest zywa i ma 2 lub 3 sasiadow to pozostaje zywa
      if (cell != 0 and (livingNeighbours == 2 or livingNeighbours == 3)) {
        newCell = cell; 
      }

      nextBoard[i][j] = newCell;
    }
  }
}

int main() {
  // Inicjalizacja planszy
  initializeBoard();

  for (int i=0; i<100; i++) {
    // Reguly iteracja
    iteration();
    // Zapisywanie currentBoard do obrazka
    saveBoard(i);
    // Czyszczenie tablic
    flipBoard();
  }

  return 0;
}
