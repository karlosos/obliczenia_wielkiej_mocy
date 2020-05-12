#include <iostream>
#include <tbb/tbb.h>
#include <fstream>
#include <sstream>
#include <stdio.h>

using namespace std;

unsigned char*** color;
unsigned char*** colorCopy;
int height;
int width;

double average[9] = {
  1.0, 1.0, 1.0,
  1.0, 1.0, 1.0,
  1.0, 1.0, 1.0,
};

double blur[25] = {
  0.0, 1.0, 2.0, 1.0, 0.0,
  1.0, 4.0, 8.0, 4.0, 1.0,
  2.0, 8.0, 16.0, 8.0, 2.0,
  1.0, 4.0, 8.0, 4.0, 1.0,
  0.0, 1.0, 2.0, 1.0, 0.0,
};

double sharpen [9] = {
  -1.0, -1.0, -1.0,
  -1.0, 9.0, -1.0,
  -1.0, -1.0, -1.0
};

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

void filter(double* mask, int maskWidth, int maskSum) {
  colorCopy = new unsigned char**[width];
  for (int i=0; i<width; i++) {
    colorCopy[i] = new unsigned char*[height];
    for (int j=0; j<height; j++) {
      colorCopy[i][j] = new unsigned char[3];
      for (int k=0; k<3; k++) {
        colorCopy[i][j][k] = color[i][j][k];
      }
    }
  }

  int offset = maskWidth/2;

  for (int i=0; i<width; i++) {
    for (int j=0; j<height; j++) { 
      int sum_0 = 0;
      int sum_1 = 0;
      int sum_2 = 0;

      for (int mask_i = 0; mask_i < maskWidth; mask_i++) {
        for (int mask_j = 0; mask_j < maskWidth; mask_j++) {
          int image_i = i + mask_i - offset;
          int image_j = j + mask_j - offset;   
          if (image_i < 0 || image_j < 0 || image_i >= width || image_j >= height) {
            sum_0 += 0;
            sum_1 += 0;
            sum_2 += 0; 
          } else {
            int multiplier = mask[mask_i * maskWidth + mask_j]; 
            //multiplier = 1;
            sum_0 += multiplier * color[image_i][image_j][0]; 
            sum_1 += multiplier * color[image_i][image_j][1]; 
            sum_2 += multiplier * color[image_i][image_j][2]; 
          } 
        } 
      }

      int output_0 = sum_0/maskSum;
      int output_1 = sum_1/maskSum;
      int output_2 = sum_2/maskSum;

      if (output_0 < 0) {
        output_0 = 0;
      } else if (output_0 > 255) {
        output_0 = 255;      
      }

      if (output_1 < 0) {
        output_1 = 0;
      } else if (output_1 > 255) {
        output_1 = 255;      
      }

      if (output_2 < 0) {
        output_2 = 0;
      } else if (output_2 > 255) {
        output_2 = 255;      
      }

      colorCopy[i][j][0] = output_0;
      colorCopy[i][j][1] = output_1;
      colorCopy[i][j][2] = output_2;
    }
  } 
}

void save_to_file(unsigned char*** image) {
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
      fwrite(image[i][j], 1, 3, fp);
    }
  }
  fclose(fp);

  /*deallocation*/
  for (int i=0; i<width; i++) {
    for (int j=0; j<height; j++) {
      delete[] color[i][j];
      delete[] colorCopy[i][j];
    }
    delete[] color[i];
    delete[] colorCopy[i];
  }
  delete[] color;
  delete[] colorCopy;
}


int main() {
  load_image();
  filter(sharpen, 3, 1);
  save_to_file(colorCopy);
  return 0;
}
