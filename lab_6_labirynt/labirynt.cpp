/*
 * Get maze from http://www.delorie.com/game-room/mazes/genmaze.cgi and save it to maze.txt
 * Change wall characters to 'x'. 
 * Change imageWidth constant to maze size.
 */
#include <iostream>
#include <fstream>
#include <thread>
#include <math.h>
#include <algorithm>

const int imageWidth = 61;
const int imageHeight = 41;
bool maze[imageHeight][imageWidth];
unsigned char image[imageHeight][imageWidth][3];

void loadMaze() {
  std::string line;
  std::ifstream mazeFile;
  mazeFile.open("maze.txt");
  int rowIndex = 0;
  while(!mazeFile.eof() && rowIndex < imageHeight) {
    std::getline(mazeFile, line);
    for (int i = 0; i<imageWidth; i++) {
      if (line.at(i) == 'x') {
        maze[rowIndex][i] = 1;
      } else {
        maze[rowIndex][i] = 0;
      }
    }
    rowIndex++;
  }
	mazeFile.close();
}

void generateImage() {
  for (int i=0; i<imageHeight; i++) {
    for (int j=0; j<imageWidth; j++) {
      if (maze[i][j]) {  
        image[i][j][0] = 220;
        image[i][j][1] = 80;
        image[i][j][2] = 90;
      }
    }
  }
}

int main() {
    loadMaze();
    generateImage();
    FILE *fp;
    char *filename = "new1.ppm";
    char *comment = "# ";
    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, imageWidth, imageHeight, 255);

    fwrite(image, 1, 3 * imageHeight * imageWidth, fp);
    fclose(fp);
    return 0;
}
