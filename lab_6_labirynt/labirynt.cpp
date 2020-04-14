/*
 * Get maze from http://www.delorie.com/game-room/mazes/genmaze.cgi and save it to maze.txt
 * Change wall characters to 'x'. 
 * Change imageWidth and imageHeight constants to maze size.
 */
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <math.h>
#include <vector>
#include <utility>
#include <algorithm>

const int imageWidth = 769;
const int imageHeight = 401;

int maze[imageHeight][imageWidth];
std::mutex mazeMutex[imageHeight][imageWidth];

unsigned char image[imageHeight][imageWidth][3];
std::vector<std::vector<int>> colors;

int threadCounter = 0;
std::mutex threadCounterMutex;

void loadMaze() {
  std::string line;
  std::ifstream mazeFile;
  mazeFile.open("maze4.txt");
  int rowIndex = 0;
  while(!mazeFile.eof() && rowIndex < imageHeight) {
    std::getline(mazeFile, line);
    for (int i = 0; i<imageWidth; i++) {
      if (line.at(i) == 'x') {
        maze[rowIndex][i] = -1;
      } else {
        maze[rowIndex][i] = 0;
      }
    }
    rowIndex++;
  }
	mazeFile.close();
}

void generateImage() {
  // Generate walls in image
  for (int i=0; i<imageHeight; i++) {
    for (int j=0; j<imageWidth; j++) {
      if (maze[i][j] == -1) {  
        image[i][j][0] = 0;
        image[i][j][1] = 0;
        image[i][j][2] = 0;
      } else {
        image[i][j][0] = 255;
        image[i][j][1] = 255;
        image[i][j][2] = 255;
      }
    }
  }
}

int getThreadIndex() {
  const std::lock_guard<std::mutex> lock(threadCounterMutex);
  threadCounter++;
  return threadCounter;
}

bool checkElement(int x, int y) {
  const std::lock_guard<std::mutex> lock(mazeMutex[x][y]);
  return maze[x][y] == 0;
}

bool writeToMaze(int x, int y, int value) {
  const std::lock_guard<std::mutex> lock(mazeMutex[x][y]);
  if (maze[x][y] == 0) {
    maze[x][y] = value;
    auto color = colors.at(value % colors.size());
    //char color = value % 120 + 100;
    image[x][y][0] = color[0];
    image[x][y][1] = color[1];
    image[x][y][2] = color[2];
    return true;
  } else {
    return false;
  }
}

void mazeRun(int start_position_x, int start_position_y) {
  int x = start_position_x;
  int y = start_position_y;
  int index = getThreadIndex(); 
  std::vector<std::thread> children;
  bool canMove = true;
  while (canMove) {
    canMove = writeToMaze(x, y, index);
    if (!canMove) {
      break;
    }

    std::vector<std::pair<int, int>> availableWays;
    if (x-1 >= 0) {
      if (checkElement(x-1, y))
        availableWays.push_back(std::make_pair<int, int>(x-1, y+0));
    }
    if (x+1 < imageHeight) {
      if (checkElement(x+1, y))
        availableWays.push_back(std::make_pair<int, int>(x+1, y+0));
    }
    if (y-1 >= 0) {
      if (checkElement(x, y-1))
        availableWays.push_back(std::make_pair<int, int>(x+0, y-1));
    }
    if (y+1 < imageWidth) {
      if (checkElement(x, y+1))
        availableWays.push_back(std::make_pair<int, int>(x+0, y+1));
    }

    if (availableWays.size() == 1) {
      x = availableWays.at(0).first;
      y = availableWays.at(0).second;
    }
    else if(availableWays.size() == 0) {
      canMove = false;
    }
    else {
      x = availableWays.at(0).first;
      y = availableWays.at(0).second;
      for (int i=1; i<availableWays.size(); i++) {
        int child_x = availableWays.at(i).first;
        int child_y = availableWays.at(i).second;
        children.push_back(std::thread(mazeRun, child_x, child_y));
      }
    }
  }

  for (auto &child : children) {
    child.join(); 
  }
}

void generateColors() {
  int r, g, b;
  r = 250;
  g = 0;
  b = 0;
  int step = 1;
  for (int i=0; i<(250/step)*3; i++) {
    if(r > 0 && b == 0){
      r -= step;
      g += step;
    }
    if(g > 0 && r == 0){
      g -= step;
      b += step;
    }
    if(b > 0 && g == 0){
      r += step;
      b -= step;
    }
    std::vector<int> color = {r, g, b};
    colors.push_back(color);
  }
}

int main() {
    loadMaze();
    generateColors();
    generateImage();
    FILE *fp;
    char *filename = "new1.ppm";
    char *comment = "# ";
    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, imageWidth, imageHeight, 255);

    // startowy element (1, 0)
    std::thread mazeRunner(mazeRun, 1, 0);
    mazeRunner.join();
    std::cout << "Koniec :-) " << threadCounter << std::endl;

    fwrite(image, 1, 3 * imageHeight * imageWidth, fp);
    fclose(fp);
    return 0;
}
