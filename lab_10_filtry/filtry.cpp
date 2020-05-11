#include <iostream>
#include <tbb/tbb.h>
#include <fstream>
#include <sstream>
#include <stdio.h>

using namespace std;

unsigned char*** color;
int height;
int width;

void load_image() {
  FILE *fp;
  const char *filename = "lenna.ppm";
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  /*reading metadata*/
  fp = fopen(filename, "rb");
  getline(&line, &len, fp); // P6
  getline(&line, &len, fp); // width
  string sizes(line);
  getline(&line, &len, fp); // max component value
  string componentValueString(line);

  /*getting size*/
  sscanf(sizes.c_str(), "%d %d", &width, &height);
  cout << width << ", " << height << endl;

  /*allocation*/
  color = new unsigned char**[width];
  for (int i=0; i<width; i++) {
    color[i] = new unsigned char*[height];
    for (int j=0; j<height; j++) {
      color[i][j] = new unsigned char[3];
    }
  }

  /*reading data*/
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      fread(color[i][j], 1, 3, fp);
    }
  }

  free(line);
  fclose(fp);
}

void save_to_file() {
  const int MaxColorComponentValue = 255;
  FILE *fp;
  const char *filename = "output.ppm";
  char *comment = "# ";
  fp = fopen(filename, "wb");
  fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, width, height, MaxColorComponentValue);

  /*write color to the file*/
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      fwrite(color[i][j], 1, 3, fp);
    }
  }
  fclose(fp);

  /*deallocation*/
  for (int i=0; i<width; i++) {
    for (int j=0; j<height; j++) {
      delete[] color[i][j];
    }
    delete[] color[i];
  }
  delete[] color;
}


int main() {
  load_image();
  save_to_file();
  return 0;
}
